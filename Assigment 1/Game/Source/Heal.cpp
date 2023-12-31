#include "Item.h"
#include "Heal.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Heal::Heal() : Entity(EntityType::HEAL)
{
	name.Create("Heal");
}

Heal::~Heal() {}

bool Heal::Awake() {

	//initilize variables
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturepath = parameters.attribute("texturepath").as_string();

	Idle.LoadAnimation("Heal", "KitIdle");
	Pick.LoadAnimation("Heal", "KitPicked");

	return true;
}

bool Heal::Start() {

	//initilize textures
	texture = app->tex->Load(texturepath);
	pbody = app->physics->CreateRectangle(position.x, position.y, 16, 13, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::HEAL;

	return true;
}

bool Heal::Update(float dt)
{
	Idle.speed = 0.01f * dt;
	Pick.speed = 0.01f * dt;

	switch (state)
	{
	case HealState::IDLE:
		currentAnimation = &Idle;
		break;
	case HealState::PICKED:
		currentAnimation = &Pick;
		break;
	}

	if (isAlive) {
		//Draw the texture
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y+3, &rect);
	}

	else
	{
		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		if (currentAnimation->GetCurrentFrameIndex() >= 6)
		{
			app->entityManager->DestroyEntity(this);
			app->physics->ChupaBody(app->physics->GetWorld(), pbody->body);
		}
	}
	
	return true;
}

void Heal::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision HEAL con PLAYER");
		if(app->scene->playerAlive) isAlive = false;
		state = HealState::PICKED;
		break;
	}
		
}

bool Heal::CleanUp()
{
	return true;
}