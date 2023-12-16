#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	//initilize variables
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturepath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Item::Start() {

	//initilize textures
	texture = app->tex->Load(texturepath);
	pbody = app->physics->CreateRectangle(position.x, position.y, 12, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ITEM;

	return true;
}

bool Item::Update(float dt)
{
	if (isAlive) {
		//Draw the texture
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

		app->render->DrawTexture(texture, position.x, position.y);
	}

	if (!isAlive)
	{
		this->Disable();
		app->physics->ChupaBody(app->physics->GetWorld(), pbody->body);
	}
	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision ITEM con PLAYER");
		if(app->scene->playerAlive) isAlive = false;
		break;
	}
		
}

bool Item::CleanUp()
{
	return true;
}