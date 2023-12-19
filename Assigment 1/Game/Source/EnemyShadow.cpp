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
	//inicializa parametros
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturepath = parameters.attribute("texturepath").as_string();


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
	texture = app->tex->Load(texturepath);

	pbody = app->physics->CreateCircle(position.x, position.y + 16, 9, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;
	pbody->body->SetGravityScale(0.0f); // Reducir la influencia de la gravedad

	// Texture to highligh mouse position 
	shot = app->audio->LoadFx("EnemyShadow", "shot");
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	enemyOriginTile = app->map->WorldToMap(15 + position.x, 30 + position.y - app->render->camera.y);

	return true;
}

bool EnemyShadow::Update(float dt)
{
	Idle_right.speed = 0.01f * dt;
	Idle_left.speed = 0.01f * dt;
	Attack_right.speed = 0.01f * dt;
	Attack_left.speed = 0.01f * dt;
	Die_right.speed = 0.01f * dt;
	Die_left.speed = 0.01f * dt;
	cd -= dt * 0.1;

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  


	if (isAlive) {
		if (!isAttackingLeft && !isAttackingRight && !isMovingLeft && !isMovingRight) {
			if (currentDirection == EnemyShadowDirection::RIGHT || currentDirection == EnemyShadowDirection::ATTACK_R)
			{
				currentDirection = EnemyShadowDirection::RIGHT;
			}
			else if (currentDirection == EnemyShadowDirection::LEFT || currentDirection == EnemyShadowDirection::ATTACK_L)
			{
				currentDirection = EnemyShadowDirection::LEFT;
			}
		}

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 22;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 30;

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
			app->map->pathfinding2->CreatePath(enemyTile, app->scene->playerTile, app->map->pathfinding2);
			path = app->map->pathfinding2->GetLastPath();		// L13: Get the latest calculated path and draw
		}

		else {
			seePlayer = false;
			isAttackingLeft = false;
			isAttackingRight = false;
			app->map->pathfinding2->CreatePath(enemyTile, enemyOriginTile, app->map->pathfinding2);
			path = app->map->pathfinding2->GetLastPath();		// L13: Get the latest calculated path and draw
		}

		if (app->physics->debug && path != NULL) {
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
			}
		}
		
		if (position.x - app->scene->getPlayerPos().x <= 100 && position.x - app->scene->getPlayerPos().x >= -100 && 
			position.y - app->scene->getPlayerPos().y >= -30 && position.y - app->scene->getPlayerPos().y <= 30 && cd <= 0.0f) {
			attack();
		}

		pbody->body->SetLinearVelocity(vel);

		//esta parte del codigo hace que el enemigo se mueva hacia el jugador cuando lo ve

		if (seePlayer && !isAttackingLeft && !isAttackingRight) {
			if (position.x - app->scene->getPlayerPos().x < 0) {
				isMovingRight = true;
				isMovingLeft = false;
				currentDirection = EnemyShadowDirection::RIGHT;
			}
			else if (position.x - app->scene->getPlayerPos().x > 0) {
				isMovingLeft = true;
				isMovingRight = false;
				currentDirection = EnemyShadowDirection::LEFT;
			}
		}

		enemyTile = app->map->WorldToMap(21 + position.x, 30 + position.y - app->render->camera.y);
		
		if(isMovingLeft||isMovingRight){
			MoveTowardsNextNode(enemyTile,speed,path);
		}

		if (isAttackingLeft && Attack_left.GetCurrentFrameIndex() == 8) {
			app->scene->particulas->Shoot(true, position.x-2, position.y+16, -1, ColliderType::ENEMY_SHOT);
			app->audio->PlayFx(shot);
			isAttackingLeft = false;
			Attack_left.Reset();
		}
		if (isAttackingRight && Attack_right.GetCurrentFrameIndex() == 8) {
			app->scene->particulas->Shoot(true, position.x+42, position.y+16, 1, ColliderType::ENEMY_SHOT);
			app->audio->PlayFx(shot);
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
			app->entityManager->DestroyEntity(this);
			app->physics->ChupaBody(app->physics->GetWorld(), pbody->body);
		}

	}

	return true;
}

void EnemyShadow :: MoveTowardsNextNode(iPoint& enemyTile, float speed, const DynArray<iPoint>* path) //esta funcion sirve para que el enemigo se mueva hacia el siguiente nodo
{
	b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo
	if (path->Count() > 0) {
		iPoint nextNode;
		if (app->map->pathfinding2->Move(enemyTile, nextNode)) {
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
	// Liberar recursos y realizar cualquier limpieza necesaria
	app->tex->UnLoad(texture);

	return true;
}

void EnemyShadow::attack() {

	if (position.x - app->scene->getPlayerPos().x >= 0) {
		isAttackingLeft = true;
		currentDirection = EnemyShadowDirection::ATTACK_L;
	}
	else if (position.x - app->scene->getPlayerPos().x < 0) {
		isAttackingRight = true;
		currentDirection = EnemyShadowDirection::ATTACK_R;
	}
	cd = 200.0f;
}
