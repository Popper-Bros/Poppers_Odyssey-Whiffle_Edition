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
#include "Map.h"

EnemyZombie::EnemyZombie() : Entity(EntityType::ENEMYZOMBIE)
{
	name.Create("EnemyZombie");
}

EnemyZombie::~EnemyZombie() {}

bool EnemyZombie::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturepath = parameters.attribute("texturepath").as_string();

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
	texture = app->tex->Load(texturepath);

	pbody = app->physics->CreateCircle(position.x, position.y + 16, 9, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	roar = app->audio->LoadFx("EnemyZombie", "attack");
	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");


	enemyOriginTile = app->map->WorldToMap(12 + position.x, 30 + position.y - app->render->camera.y);

	return true;
}

bool EnemyZombie::Update(float dt)
{
	Idle_right.speed = 0.01f * dt;
	Idle_left.speed = 0.01f * dt;
	Attack_right.speed = 0.01f * dt;
	Attack_left.speed = 0.01f * dt;
	Die_right.speed = 0.01f * dt;
	Die_left.speed = 0.01f * dt;
	Move_right.speed = 0.01f * dt;
	Move_left.speed = 0.01f * dt;

	cd -= dt * 0.1;
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	if (isAlive) {

		if (!isAttackingLeft && !isAttackingRight && !isMovingLeft && !isMovingRight) {
			if (currentDirection == EnemyZombieDirection::RIGHT || currentDirection == EnemyZombieDirection::ATTACK_R)
			{
				currentDirection = EnemyZombieDirection::IDLE_R;
			}
			else if (currentDirection == EnemyZombieDirection::LEFT || currentDirection == EnemyZombieDirection::ATTACK_L)
			{
				currentDirection = EnemyZombieDirection::IDLE_L;
			}
		}

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 33;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 50;

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

		if (position.x - app->scene->getPlayerPos().x <= 200 && position.x - app->scene->getPlayerPos().x >= -200) {
			seePlayer = true;
			app->map->pathfinding->CreatePath(enemyTile, app->scene->playerTile, app->map->pathfinding);
			path = app->map->pathfinding->GetLastPath();		// L13: Get the latest calculated path and draw
		}

		else {
			seePlayer = false;
			isAttackingLeft = false;
			isAttackingRight = false;
			app->map->pathfinding->CreatePath(enemyTile, enemyOriginTile, app->map->pathfinding);
			path = app->map->pathfinding->GetLastPath();		// L13: Get the latest calculated path and draw
		}

		if (app->physics->debug && path!=NULL) {
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
			}
		}

		if (position.x - app->scene->getPlayerPos().x <= 40 && position.x - app->scene->getPlayerPos().x >= -40 && cd<=0.0f) {
			attack();
		}

		pbody->body->SetLinearVelocity(vel);

		if (seePlayer && !isAttackingLeft && !isAttackingRight) {
			if (position.x - app->scene->getPlayerPos().x < 0) {
				isMovingRight = true;
				isMovingLeft = false;
			}
			else if (position.x - app->scene->getPlayerPos().x > 0) {
				isMovingLeft = true;
				isMovingRight = false;
			}
		}

		enemyTile = app->map->WorldToMap(20 + position.x, 30 + position.y - app->render->camera.y);

		if(isMovingLeft||isMovingRight){
			MoveTowardsNextNode(enemyTile,speed,path,vel.y);
		}

		if (isAttackingLeft && Attack_left.GetCurrentFrameIndex() == 4) {
			isAttackingLeft = false;
			Attack_left.Reset();
		}
		if (isAttackingRight && Attack_right.GetCurrentFrameIndex() == 4) {
			isAttackingRight = false;
			Attack_right.Reset();
		}
		
		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}

	else
	{
		
		pbody->body->GetFixtureList()->SetSensor(true);
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

void EnemyZombie::MoveTowardsNextNode(iPoint& enemyTile, float speed, const DynArray<iPoint>* path, float vely) {
	b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo
	if (path->Count() > 0) {
		iPoint nextNode;
		if (app->map->pathfinding->Move(enemyTile, nextNode)) {
			// Determine direction from the player to the next node
			int dx = nextNode.x - enemyTile.x;
			int dy = nextNode.y - enemyTile.y;

			// Determine the direction based on the sign of dx and dy
			if (dx > 0) {
				vel = { speed,vely };
				currentDirection = EnemyZombieDirection::RIGHT;
			}
			else if (dx < 0) {
				vel = { -speed,vely };
				currentDirection = EnemyZombieDirection::LEFT;
			}
			else if (dx == 0) {
				vel = { 0,vely };
				if (currentDirection == EnemyZombieDirection::LEFT || currentDirection == EnemyZombieDirection::IDLE_L) {
					currentDirection = EnemyZombieDirection::IDLE_L;
				}
				else if (currentDirection == EnemyZombieDirection::RIGHT || currentDirection == EnemyZombieDirection::IDLE_R) {
					currentDirection = EnemyZombieDirection::IDLE_R;
				}
			}

			enemyTile = nextNode;
		}
	}
	pbody->body->SetLinearVelocity(vel);
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

void EnemyZombie::attack() {

	if (position.x - app->scene->getPlayerPos().x >= 0) {
		isAttackingLeft = true;
		app->audio->PlayFx(roar);
		currentDirection = EnemyZombieDirection::ATTACK_L;
	}
	else if (position.x - app->scene->getPlayerPos().x < 0) {
		isAttackingRight = true;
		app->audio->PlayFx(roar);
		currentDirection = EnemyZombieDirection::ATTACK_R;
	}
	cd = 200.0f;
}