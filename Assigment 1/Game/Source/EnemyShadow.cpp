#include "EnemyShadow.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Item.h"
#include "Player.h"

EnemyShadow::EnemyShadow() : Entity(EntityType::ENEMYSHADOW)
{
	name.Create("EnemyShadow");
}

EnemyShadow::~EnemyShadow() {}

bool EnemyShadow::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePathEnemyShadow = parameters.attribute("enemyShadowTexturepath").as_string();

	return true;
}

bool EnemyShadow::Start() {

	//initilize textures
	texture = app->tex->Load(texturePathEnemyShadow);

	pbody = app->physics->CreateCircle(position.x, position.y + 16, 9, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	Move_right.PushBack({ 19+(50*0), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 1), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 2), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 3), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 4), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 5), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 6), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 7), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 8), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 9), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 10), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 11), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 12), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 13), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 14), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 15), 16, 48, 48 });

	Idle_right.PushBack({ 19 + (50 * 0), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 1), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 2), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 3), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 4), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 5), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 6), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 7), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 8), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 9), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 10), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 11), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 12), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 13), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 14), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 15), 16, 48, 48 });

	Move_left.PushBack({ 769 - (50 * 0), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 1), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 2), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 3), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 4), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 5), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 6), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 7), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 8), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 9), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 10), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 11), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 12), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 13), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 14), 66, 48, 48 });
	Move_left.PushBack({ 769 - (50 * 15), 66, 48, 48 });

	Idle_left.PushBack({ 769 - (50 * 0), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 1), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 2), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 3), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 4), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 5), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 6), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 7), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 8), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 9), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 10), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 11), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 12), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 13), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 14), 66, 48, 48 });
	Idle_left.PushBack({ 769 - (50 * 15), 66, 48, 48 });

	Die_right.PushBack({ 19 + (50 * 0), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 1), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 2), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 3), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 4), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 5), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 6), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 7), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 8), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 9), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 10), 116, 48, 48 });
	Die_right.PushBack({ 19 + (50 * 11), 116, 48, 48 });
	Die_right.loop = false;

	Die_left.PushBack({ 569 - (50 * 0), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 1), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 2), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 3), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 4), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 5), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 6), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 7), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 8), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 9), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 10), 166, 48, 48 });
	Die_left.PushBack({ 569 - (50 * 11), 166, 48, 48 });
	Die_left.loop = false;

	Attack_right.PushBack({ 20 + (45 * 0), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 1), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 2), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 3), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 4), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 5), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 6), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 7), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 8), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 9), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 10), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 11), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 12), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 13), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 14), 223, 43, 45 });
	Attack_right.PushBack({ 20 + (45 * 15), 223, 43, 45 });

	Attack_left.PushBack({ 695 - (45 * 0), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 1), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 2), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 3), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 4), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 5), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 6), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 7), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 8), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 9), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 10), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 11), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 12), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 13), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 14), 270, 43, 45 });
	Attack_left.PushBack({ 695 - (45 * 15), 270, 43, 45 });

	return true;
}

bool EnemyShadow::Update(float dt)
{
	Move_right.speed = 0.01f * dt;
	Move_left.speed = 0.01f * dt;
	Idle_right.speed = 0.01f * dt;
	Idle_left.speed = 0.01f * dt;
	Jump_right.speed = 0.01f * dt;
	Jump_left.speed = 0.01f * dt;
	Die_left.speed = 0.01f * dt;
	Die_right.speed = 0.01f * dt;
	Attack_right.speed = 0.01f * dt;
	Attack_left.speed = 0.01f * dt;
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 22;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 30;

	if (isAlive) {

		if (currentDirection == EnemyShadowDirection::RIGHT)
		{
			currentDirection = EnemyShadowDirection::IDLE_R;
		}
		else if (currentDirection == EnemyShadowDirection::LEFT)
		{
			currentDirection = EnemyShadowDirection::IDLE_L;
		}


		switch (currentDirection)
		{
		case EnemyShadowDirection::IDLE_R:
			currentAnimation = &Idle_right;
			break;
		case EnemyShadowDirection::IDLE_L:
			currentAnimation = &Idle_left;
			break;
		case EnemyShadowDirection::LEFT:
			currentAnimation = &Move_left;
			break;
		case EnemyShadowDirection::RIGHT:
			currentAnimation = &Move_right;
			break;
		case EnemyShadowDirection::ATTACK_R:
			currentAnimation = &Attack_right;
			break;
		case EnemyShadowDirection::ATTACK_L:
			currentAnimation = &Attack_left;
			break;

		}

		b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo

		if (((position.x - app->scene->getPlayerPos().x < 200 && position.x - app->scene->getPlayerPos().x >= 100) || (position.x - app->scene->getPlayerPos().x > -200 && position.x - app->scene->getPlayerPos().x <= -100))&&isAttacking == false){
			
			if (position.x - app->scene->getPlayerPos().x >= 0) {
				currentDirection = EnemyShadowDirection::LEFT;
				isMovingLeft = true;
			}
			else {
				currentDirection = EnemyShadowDirection::RIGHT;
				isMovingRight = true;
			}
		}

		if (isMovingLeft) {
			vel.x = -speed * dt;
		}

		if (isMovingRight) {
			vel.x = speed * dt;
		}

		if (((position.x - app->scene->getPlayerPos().x < 100 && position.x - app->scene->getPlayerPos().x >= 0) || (position.x - app->scene->getPlayerPos().x > -100 && position.x - app->scene->getPlayerPos().x < 0)) && (position.y - app->scene->getPlayerPos().y >= -30 && position.y - app->scene->getPlayerPos().y <= 30)) {
			isAttacking = true;
			isMovingLeft = false;
			isMovingRight = false;
		}

		pbody->body->SetLinearVelocity(vel);

		if(isAttacking){
			if (position.x - app->scene->getPlayerPos().x >= 0) {
				currentDirection = EnemyShadowDirection::ATTACK_L;
			}
			if (position.x - app->scene->getPlayerPos().x < 0) {
				currentDirection = EnemyShadowDirection::ATTACK_R;
			}
			if (currentAnimation->GetCurrentFrameIndex() >= 15)
			{
				isAttacking = false;
			}
		}
		
		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}

	else
	{
		//pbody->body->GetFixtureList()->SetSensor(true);
		//if (currentAnimation->GetCurrentFrameIndex() >= 0) app->audio->PlayFx(blood, 24);
		if (currentAnimation == &Idle_right || currentAnimation == &Move_right) {
			currentAnimation = &Die_right;
		}
		if (currentAnimation == &Idle_left || currentAnimation == &Move_left) {
			currentAnimation = &Die_left;
		}
		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);

		if (currentAnimation->GetCurrentFrameIndex() >= 11)
		{
			Disable();
			app->physics->ChupaBody(app->physics->GetWorld(), pbody->body);
		}

	}


	return true;
}

void EnemyShadow::OnCollision(PhysBody* physA, PhysBody* physB)
{
	b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo

	switch (physB->ctype)
	{
	
	case ColliderType::SPIKES:
		//isAlive = false;
		LOG("Collision SPIKES");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
		
}

bool EnemyShadow::CleanUp()
{
	return true;
}