#include "EnemyZombie.h"
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

EnemyZombie::EnemyZombie() : Entity(EntityType::ENEMYZOMBIE)
{
	name.Create("EnemyZombie");
}

EnemyZombie::~EnemyZombie() {}

bool EnemyZombie::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePathEnemyZombie = parameters.attribute("enemyZombieTexturepath").as_string();

	Idle_right.LoadAnimation("EnemyZombie", "Idle_right");
	Idle_left.LoadAnimation("EnemyZombie", "Idle_left");
	Attack_right.LoadAnimation("EnemyZombie", "Attack_right");
	Attack_left.LoadAnimation("EnemyZombie", "Attack_left");
	Die_right.LoadAnimation("EnemyZombie", "Die_right");
	Die_left.LoadAnimation("EnemyZombie", "Die_left");
	Move_right.LoadAnimation("EnemyZombie", "Move_right");
	Move_left.LoadAnimation("EnemyZombie", "Move_left");


	return true;
}

bool EnemyZombie::Start() {

	//initilize textures
	texture = app->tex->Load(texturePathEnemyZombie);

	pbody = app->physics->CreateCircle(position.x, position.y + 16, 9, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	return true;
}

bool EnemyZombie::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 33;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 50;

	if (isAlive) {

		if (!isAttacking && !isMovingLeft && !isMovingRight) {
			if (currentDirection == EnemyZombieDirection::RIGHT || currentDirection == EnemyZombieDirection::ATTACK_R)
			{
				currentDirection = EnemyZombieDirection::IDLE_R;
			}
			else if (currentDirection == EnemyZombieDirection::LEFT || currentDirection == EnemyZombieDirection::ATTACK_L)
			{
				currentDirection = EnemyZombieDirection::IDLE_L;
			}
		}

		switch (currentDirection)
		{
		case EnemyZombieDirection::IDLE_R:
			currentAnimation = &Idle_right;
			break;
		case EnemyZombieDirection::IDLE_L:
			currentAnimation = &Idle_left;
			break;
		case EnemyZombieDirection::LEFT:
			currentAnimation = &Move_left;
			break;
		case EnemyZombieDirection::RIGHT:
			currentAnimation = &Move_right;
			break;
		case EnemyZombieDirection::ATTACK_R:
			currentAnimation = &Attack_right;
			break;
		case EnemyZombieDirection::ATTACK_L:
			currentAnimation = &Attack_left;
			break;

		}

		b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo

		if (isMovingLeft) isMovingRight = false;
		if (isMovingRight) isMovingLeft = false;

		if (((position.x - app->scene->getPlayerPos().x < 35 && position.x - app->scene->getPlayerPos().x >= 0) || (position.x - app->scene->getPlayerPos().x > -35 && position.x - app->scene->getPlayerPos().x < 0))) {
			if (position.y - app->scene->getPlayerPos().y >= -45 && position.y - app->scene->getPlayerPos().y <= 45){
				isAttacking = true;
				isMovingLeft = false;
				isMovingRight = false;
			}
		}
		else {
			isAttacking = false;
		}

		pbody->body->SetLinearVelocity(vel);

		if(isAttacking){
			if (position.x - app->scene->getPlayerPos().x >= 0) {
				currentDirection = EnemyZombieDirection::ATTACK_L;
			}
			if (position.x - app->scene->getPlayerPos().x < 0) {
				currentDirection = EnemyZombieDirection::ATTACK_R;
			}
			if (currentAnimation->GetCurrentFrameIndex() >= 4)
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

		if (currentAnimation->GetCurrentFrameIndex() >= 4)
		{
			this->Disable();
			app->physics->ChupaBody(app->physics->GetWorld(), pbody->body);
		}

	}


	return true;
}

void EnemyZombie::OnCollision(PhysBody* physA, PhysBody* physB)
{
	b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo

	switch (physB->ctype)
	{
	
	case ColliderType::SPIKES:
		//isAlive = false;
		LOG("Collision SPIKES");
		break;
	case ColliderType::PLAYER_SHOT:
		isAlive = false;
		LOG("Collision SHOT");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
		
}

bool EnemyZombie::CleanUp()
{
	return true;
}