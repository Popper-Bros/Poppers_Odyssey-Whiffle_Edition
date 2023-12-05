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
#include "Map.h"

EnemyShadow::EnemyShadow() : Entity(EntityType::ENEMYSHADOW)
{
	name.Create("EnemyShadow");
}

EnemyShadow::~EnemyShadow() {}

bool EnemyShadow::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePathEnemyShadow = parameters.attribute("enemyShadowTexturepath").as_string();


	Idle_right.LoadAnimation("EnemyShadow", "Idle_right");
	Idle_left.LoadAnimation("EnemyShadow", "Idle_left");
	Attack_right.LoadAnimation("EnemyShadow", "Attack_right");
	Attack_left.LoadAnimation("EnemyShadow", "Attack_left");
	Die_right.LoadAnimation("EnemyShadow", "Die_right");
	Die_left.LoadAnimation("EnemyShadow", "Die_left");


	return true;
}

bool EnemyShadow::Start() {

	//initilize textures
	texture = app->tex->Load(texturePathEnemyShadow);

	pbody = app->physics->CreateCircle(position.x, position.y + 16, 9, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;


	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	return true;
}

bool EnemyShadow::Update(float dt)
{
	cd -= dt * 0.1;

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 22;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 30;

	if (isAlive) {
		if (!isAttackingRight && !isAttackingLeft) {
			/*if (currentDirection == EnemyShadowDirection::RIGHT || currentDirection == EnemyShadowDirection::ATTACK_R)
			{
				currentDirection = EnemyShadowDirection::IDLE_R;
			}
			else if (currentDirection == EnemyShadowDirection::LEFT || currentDirection == EnemyShadowDirection::ATTACK_L)
			{
				currentDirection = EnemyShadowDirection::IDLE_L;
			}*/
		}

		switch (currentDirection)
		{
		case EnemyShadowDirection::LEFT:
			currentAnimation = &Idle_left;
			break;
		case EnemyShadowDirection::RIGHT:
			currentAnimation = &Idle_right;
			break;
		case EnemyShadowDirection::ATTACK_R:
			currentAnimation = &Attack_right; 
			break;
		case EnemyShadowDirection::ATTACK_L:
			currentAnimation = &Attack_left;
			break;

		}

		b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo

		if (position.x - app->scene->getPlayerPos().x <= 200 && position.x - app->scene->getPlayerPos().x >= -200) {
			seePlayer = true;
		}
		else {
			seePlayer = false;
		}
		
		if (seePlayer && position.x - app->scene->getPlayerPos().x <= 100 && position.x - app->scene->getPlayerPos().x >= -100 && 
			position.y - app->scene->getPlayerPos().y >= -30 && position.y - app->scene->getPlayerPos().y <= 30 && cd <= 0.0f) {
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

		enemyTile = app->map->WorldToMap(20+position.x, 25+position.y - app->render->camera.y);
		app->map->pathfinding->CreatePath(enemyTile, app->scene->playerTile);
		path = app->map->pathfinding->GetLastPath();

		// L13: Get the latest calculated path and draw
		if (app->physics->debug) {
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
			}
		}

		if(isMovingLeft||isMovingRight){
			MoveTowardsNextNode(enemyTile,speed,path);
		}

		if (isAttackingLeft && Attack_left.GetCurrentFrameIndex() == 8) {
			app->scene->particulas->Shoot(true, position.x-2, position.y+16, -1, ColliderType::ENEMY_SHOT);
			isAttackingLeft = false;
			currentDirection = EnemyShadowDirection::LEFT;
		}
		if (isAttackingRight && Attack_right.GetCurrentFrameIndex() == 8) {
			app->scene->particulas->Shoot(true, position.x+42, position.y+16, 1, ColliderType::ENEMY_SHOT);
			isAttackingRight = false;
			currentDirection = EnemyShadowDirection::RIGHT;
		}
		
		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}

	else
	{
		
		//pbody->body->GetFixtureList()->SetSensor(true);
		//if (currentAnimation->GetCurrentFrameIndex() >= 0) app->audio->PlayFx(blood, 24);
		if (currentAnimation == &Idle_right || currentAnimation == &Attack_right) {
			currentAnimation = &Die_right;
		}
		if (currentAnimation == &Idle_left || currentAnimation == &Attack_left) {
			currentAnimation = &Die_left;
		}
		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);

		if (currentAnimation->GetCurrentFrameIndex() >= 11)
		{
			this->Disable();
			app->physics->ChupaBody(app->physics->GetWorld(), pbody->body);
		}

	}

	return true;
}

void EnemyShadow :: MoveTowardsNextNode(iPoint& enemyTile, float speed, const DynArray<iPoint>* path) {
	b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo
	if (path->Count() > 0) {
		iPoint nextNode;
		if (app->map->pathfinding->Move(enemyTile, nextNode)) {
			// Determine direction from the player to the next node
			int dx = nextNode.x - enemyTile.x;
			int dy = nextNode.y - enemyTile.y;

			// Determine the direction based on the sign of dx and dy
			if (dx > 0) {
				vel = { speed,0 };
			}
			else if (dx < 0) {
				vel = { -speed,0 };
			}

			if (dy > 0) {
				vel = { 0,speed };
			}
			else if (dy < 0) {
				vel = { 0,-speed };
			}
			enemyTile = nextNode;
		}
	}
	pbody->body->SetLinearVelocity(vel);
}

void EnemyShadow::OnCollision(PhysBody* physA, PhysBody* physB)
{

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

bool EnemyShadow::CleanUp()
{
	return true;
}

void EnemyShadow::attack() {

	Attack_right.Reset();
	Attack_left.Reset();

	if (((position.x - app->scene->getPlayerPos().x <= 100 && position.x - app->scene->getPlayerPos().x >= 0))) {
		isAttackingLeft = true;
		currentDirection = EnemyShadowDirection::ATTACK_L;
	}
	else if (position.x - app->scene->getPlayerPos().x >= -100 && position.x - app->scene->getPlayerPos().x < 0) {
		isAttackingRight = true;
		currentDirection = EnemyShadowDirection::ATTACK_R;
	}
	cd = 20.0f;
}
