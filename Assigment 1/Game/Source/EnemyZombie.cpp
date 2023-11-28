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

	return true;
}

bool EnemyZombie::Start() {

	//initilize textures
	texture = app->tex->Load(texturePathEnemyZombie);

	pbody = app->physics->CreateCircle(position.x, position.y + 16, 9, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	Idle_right.PushBack({ 6 + (68 * 0), 10, 67, 67 });
	Idle_right.PushBack({ 6 + (68 * 1), 10, 67, 67 });
	Idle_right.PushBack({ 6 + (68 * 2), 10, 67, 67 });
	Idle_right.PushBack({ 6 + (68 * 3), 10, 67, 67 });
	Idle_right.PushBack({ 6 + (68 * 4), 10, 67, 67 });
	Idle_right.PushBack({ 6 + (68 * 5), 10, 67, 67 });
	Idle_right.PushBack({ 6 + (68 * 6), 10, 67, 67 });
	Idle_right.PushBack({ 6 + (68 * 7), 10, 67, 67 });
	Idle_right.loop = true;

	Move_right.PushBack({ 6 + (68 * 0), 78, 67, 67 });
	Move_right.PushBack({ 19 + (68 * 1), 78, 67, 67 });
	Move_right.PushBack({ 19 + (68 * 2), 78, 67, 67 });
	Move_right.PushBack({ 19 + (68 * 3), 78, 67, 67 });
	Move_right.PushBack({ 19 + (68 * 4), 78, 67, 67 });
	Move_right.PushBack({ 19 + (68 * 5), 78, 67, 67 });
	Move_right.PushBack({ 19 + (68 * 6), 78, 67, 67 });
	Move_right.PushBack({ 19 + (68 * 7), 78, 67, 67 });

	Move_left.PushBack({ 959 - (68 * 0), 342, 67, 67 });
	Move_left.PushBack({ 959 - (68 * 1), 342, 67, 67 });
	Move_left.PushBack({ 959 - (68 * 2), 342, 67, 67 });
	Move_left.PushBack({ 959 - (68 * 3), 342, 67, 67 });
	Move_left.PushBack({ 959 - (68 * 4), 342, 67, 67 });
	Move_left.PushBack({ 959 - (68 * 5), 342, 67, 67 });
	Move_left.PushBack({ 959 - (68 * 6), 342, 67, 67 });
	Move_left.PushBack({ 959 - (68 * 7), 342, 67, 67 });

	Idle_left.PushBack({ 959 - (68 * 0), 274, 67, 67 });
	Idle_left.PushBack({ 959 - (68 * 1), 274, 67, 67 });
	Idle_left.PushBack({ 959 - (68 * 2), 274, 67, 67 });
	Idle_left.PushBack({ 959 - (68 * 3), 274, 67, 67 });
	Idle_left.PushBack({ 959 - (68 * 4), 274, 67, 67 });
	Idle_left.PushBack({ 959 - (68 * 5), 274, 67, 67 });
	Idle_left.PushBack({ 959 - (68 * 6), 274, 67, 67 });
	Idle_left.PushBack({ 959 - (68 * 7), 274, 67, 67 });
	Idle_left.loop = true;

	Die_right.PushBack({ 8 + (68 * 0), 628, 67, 67 });
	Die_right.PushBack({ 8 + (68 * 1), 628, 67, 67 });
	Die_right.PushBack({ 8 + (68 * 2), 628, 67, 67 });
	Die_right.PushBack({ 8 + (68 * 3), 628, 67, 67 });
	Die_right.PushBack({ 8 + (68 * 4), 628, 67, 67 });
	Die_right.loop = false;

	Die_left.PushBack({ 957 - (68 * 0), 892, 67, 67 });
	Die_left.PushBack({ 957 - (68 * 1), 892, 67, 67 });
	Die_left.PushBack({ 957 - (68 * 2), 892, 67, 67 });
	Die_left.PushBack({ 957 - (68 * 3), 892, 67, 67 });
	Die_left.PushBack({ 957 - (68 * 4), 892, 67, 67 });
	Die_left.loop = false;

	Attack_left.PushBack({ 957 - (68 * 0), 549, 67, 67 });
	Attack_left.PushBack({ 957 - (68 * 1), 549, 67, 67 });
	Attack_left.PushBack({ 957 - (68 * 2), 549, 67, 67 });
	Attack_left.PushBack({ 957 - (68 * 3), 549, 67, 67 });
	Attack_left.PushBack({ 957 - (68 * 4), 549, 67, 67 });

	Attack_right.PushBack({ 8 + (68 * 0), 285, 67, 67 });
	Attack_right.PushBack({ 8 + (68 * 1), 285, 67, 67 });
	Attack_right.PushBack({ 8 + (68 * 2), 285, 67, 67 });
	Attack_right.PushBack({ 8 + (68 * 3), 285, 67, 67 });
	Attack_right.PushBack({ 8 + (68 * 4), 285, 67, 67 });

	return true;
}

bool EnemyZombie::Update(float dt)
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

		//if (((position.x - app->scene->getPlayerPos().x < 200 && position.x - app->scene->getPlayerPos().x >= 25) || (position.x - app->scene->getPlayerPos().x > -25 && position.x - app->scene->getPlayerPos().x <= -100))&&isAttacking == false){
		//	
		//	if (position.x - app->scene->getPlayerPos().x >= 0) {
		//		currentDirection = EnemyZombieDirection::LEFT;
		//		isMovingLeft = true;
		//	}
		//	else if (position.x - app->scene->getPlayerPos().x < 0) {
		//		currentDirection = EnemyZombieDirection::RIGHT;
		//		isMovingRight = true;
		//	}
		//}

		//if (isMovingLeft) {
		//	vel.x = -speed * dt;
		//}

		//if (isMovingRight) {
		//	vel.x = speed * dt;
		//}

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
	case ColliderType::SHOT:
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