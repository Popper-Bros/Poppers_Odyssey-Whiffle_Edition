#include "Boss.h"
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
#include "Timer.h"
#include "PerfTimer.h"

Boss::Boss() : Entity(EntityType::BOSS)
{
	name.Create("Boss");
}

Boss::~Boss() {}

bool Boss::Awake() {

	//inicializa parametros
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturepath = parameters.attribute("texturepath").as_string();

	Idle_right.LoadAnimation("Boss", "Idle_right");
	Idle_left.LoadAnimation("Boss", "Idle_left");
	Attack_right.LoadAnimation("Boss", "Attack_right");
	Attack_left.LoadAnimation("Boss", "Attack_left");
	Die_right.LoadAnimation("Boss", "Die_right");
	Die_left.LoadAnimation("Boss", "Die_left");
	Move_right.LoadAnimation("Boss", "Move_right");
	Move_left.LoadAnimation("Boss", "Move_left");

	EnemyHealthBar1Path = parameters.attribute("HealthBar1").as_string();
	EnemyHealthBar2Path = parameters.attribute("HealthBar2").as_string();
	EnemyHealthBar3Path = parameters.attribute("HealthBar3").as_string();
	EnemyHealthBarNullPath = parameters.attribute("HealthBarNull").as_string();
	EnemyHealthBarFullPath = parameters.attribute("HealthBarFull").as_string();

	return true;
}

bool Boss::Start() {

	EnemyHealthBarFull = app->tex->Load(EnemyHealthBarFullPath);
	EnemyHealthBarNull = app->tex->Load(EnemyHealthBarNullPath);
	EnemyHealthBar1 = app->tex->Load(EnemyHealthBar1Path);
	EnemyHealthBar2 = app->tex->Load(EnemyHealthBar2Path);
	EnemyHealthBar3 = app->tex->Load(EnemyHealthBar3Path);

	//initilize textures
	texture = app->tex->Load(texturepath);

	pbody = app->physics->CreateCircle(position.x, position.y + 16, 9, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	roar = app->audio->LoadFx("Boss", "attack");
	dead = app->audio->LoadFx("Boss", "death");
	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");


	enemyOriginTile = app->map->WorldToMap(140 + position.x, 178 + position.y - app->render->camera.y);
	health = 4;

	return true;
}

bool Boss::Update(float dt)
{

	Idle_right.speed = animSpeed * dt;
	Idle_left.speed = animSpeed * dt;
	Attack_right.speed = animSpeed * dt;
	Attack_left.speed = animSpeed * dt;
	Die_right.speed = animSpeed * dt;
	Die_left.speed = animSpeed * dt;
	Move_right.speed = animSpeed * dt;
	Move_left.speed = animSpeed * dt;

	cd -= dt * 0.1;
	EnemyHealthRec = { position.x + 20 + app->render->camera.x, position.y + app->render->camera.y, 20, 6 };

	switch (health)
	{
	case 4:
		SDL_RenderCopy(app->render->renderer, EnemyHealthBarFull, NULL, &EnemyHealthRec);
		break;
	case 3:
		SDL_RenderCopy(app->render->renderer, EnemyHealthBar3, NULL, &EnemyHealthRec);
		break;
	case 2:
		SDL_RenderCopy(app->render->renderer, EnemyHealthBar2, NULL, &EnemyHealthRec);
		break;
	case 1:
		SDL_RenderCopy(app->render->renderer, EnemyHealthBar1, NULL, &EnemyHealthRec);
		break;
	case 0:
		SDL_RenderCopy(app->render->renderer, EnemyHealthBarNull, NULL, &EnemyHealthRec);
		break;
	}

	if (isAlive) {

		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && app->scene->playerItem > 0)
		{
			timer.Start();
			timerOn = true;
			LOG("Timer started");
			speed = 0.2f;
			animSpeed = 0.002f;
		}
		if (timerOn)
		{
			if (timer.ReadSec() >= 3)
			{
				speed = 1.0f;
				animSpeed = 0.01f;
				LOG("%f", speed);
				timerOn = false;
			}
		}

		if (!isAttackingLeft && !isAttackingRight && !isMovingLeft && !isMovingRight) {
			if (currentDirection == BossDirection::RIGHT || currentDirection == BossDirection::ATTACK_R)
			{
				currentDirection = BossDirection::IDLE_R;
			}
			else if (currentDirection == BossDirection::LEFT || currentDirection == BossDirection::ATTACK_L)
			{
				currentDirection = BossDirection::IDLE_L;
			}
		}

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 164;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 150;

		switch (currentDirection)
		{
		case BossDirection::IDLE_R:
			currentAnimation = &Idle_right;
			break;
		case BossDirection::IDLE_L:
			currentAnimation = &Idle_left;
			break;
		case BossDirection::LEFT:
			currentAnimation = &Move_left;
			break;
		case BossDirection::RIGHT:
			currentAnimation = &Move_right;
			break;
		case BossDirection::ATTACK_R:
			currentAnimation = &Attack_right;
			break;
		case BossDirection::ATTACK_L:
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

		if (app->physics->debug && path != NULL) {
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
			}
		}

		if (position.x - app->scene->getPlayerPos().x <= 40 && position.x - app->scene->getPlayerPos().x >= -40 && cd <= 0.0f) {
			Attack();
		}

		pbody->body->SetLinearVelocity(vel);

		//esta parte de aqui se encarga de que el enemigo se mueva hacia el jugador si lo ve (pathfinding)
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

		enemyTile = app->map->WorldToMap(164 + position.x, 150 + position.y - app->render->camera.y);

		if (isMovingLeft || isMovingRight) {
			MoveTowardsNextNode(enemyTile, speed, path, vel.y);
		}

		if (isAttackingLeft && Attack_left.GetCurrentFrameIndex() == 4) {
			app->audio->PlayFx(roar);
			isAttackingLeft = false;
			Attack_left.Reset();
		}
		if (isAttackingRight && Attack_right.GetCurrentFrameIndex() == 4) {
			app->audio->PlayFx(roar);
			isAttackingRight = false;
			Attack_right.Reset();
		}

		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}

	else
	{
		Die();
	}

	return true;
}


void Boss::MoveTowardsNextNode(iPoint& enemyTile, float speed, const DynArray<iPoint>* path, float vely) //esta fucnion es para que el enemigo se mueva hacia el siguiente nodo del path
{
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
				currentDirection = BossDirection::RIGHT;
			}
			else if (dx < 0) {
				vel = { -speed,vely };
				currentDirection = BossDirection::LEFT;
			}
			else if (dx == 0) {
				vel = { 0,vely };
				if (currentDirection == BossDirection::LEFT || currentDirection == BossDirection::IDLE_L) {
					currentDirection = BossDirection::IDLE_L;
				}
				else if (currentDirection == BossDirection::RIGHT || currentDirection == BossDirection::IDLE_R) {
					currentDirection = BossDirection::IDLE_R;
				}
			}

			enemyTile = nextNode;
		}
	}
	pbody->body->SetLinearVelocity(vel);
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB)
{
	b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo

	switch (physB->ctype)
	{

	case ColliderType::SPIKES:
		//isAlive = false;
		LOG("Collision SPIKES");
		break;
	case ColliderType::PLAYER_SHOT:
		if (health > 0)
		{
			health -= 1;
		}
		if (health <= 0)
		{
			isAlive = false;
		}
		LOG("Collision SHOT");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}

bool Boss::CleanUp()
{
	// Liberar recursos y realizar cualquier limpieza necesaria
	app->tex->UnLoad(texture);
	app->tex->UnLoad(EnemyHealthBarFull);
	app->tex->UnLoad(EnemyHealthBarNull);
	app->tex->UnLoad(EnemyHealthBar1);
	app->tex->UnLoad(EnemyHealthBar2);
	app->tex->UnLoad(EnemyHealthBar3);

	return true;
}

void Boss::Attack() {

	if (position.x - app->scene->getPlayerPos().x >= 0) {
		isAttackingLeft = true;
		currentDirection = BossDirection::ATTACK_L;
	}
	else if (position.x - app->scene->getPlayerPos().x < 0) {
		isAttackingRight = true;
		currentDirection = BossDirection::ATTACK_R;
	}

	cd = 500.0f;
}

bool Boss::Die() {
	pbody->body->GetFixtureList()->SetSensor(true);
	if (currentAnimation == &Idle_right || currentAnimation == &Move_right) {
		currentAnimation = &Die_right;
	}
	if (currentAnimation == &Idle_left || currentAnimation == &Move_left) {
		currentAnimation = &Die_left;
	}
	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	app->audio->PlayFx(dead);
	if (currentAnimation->GetCurrentFrameIndex() >= 4)
	{
		app->entityManager->DestroyEntity(this);
		app->physics->ChupaBody(app->physics->GetWorld(), pbody->body);
	}
	return true;
}