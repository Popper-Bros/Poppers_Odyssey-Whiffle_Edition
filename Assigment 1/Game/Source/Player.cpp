#include "Player.h"
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
#include "Map.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {
	//se cargan los parametros del xml en el que se encuentra la informacion del jugador
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturepath = parameters.attribute("texturepath").as_string();

	Move_right.LoadAnimation("player", "Move_right");
	Move_left.LoadAnimation("player", "Move_left");
	Idle_right.LoadAnimation("player", "Idle_right");
	Idle_left.LoadAnimation("player", "Idle_left");
	Jump_right.LoadAnimation("player", "Jump_right");
	Jump_left.LoadAnimation("player", "Jump_left");
	Die.LoadAnimation("player", "Die");


	
	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturepath);

	pbody = app->physics->CreateCircle(position.x, position.y + 16, 9, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
		
	
	sniff = app->audio->LoadFx("player", "sniff");
	jump = app->audio->LoadFx("player", "jump");
	fall = app->audio->LoadFx("player", "fall");
	blood = app->audio->LoadFx("player", "blood");
	shoot = app->audio->LoadFx("player", "shoot");
	reload = app->audio->LoadFx("player", "reload");
	won = app->audio->LoadFx("player", "win");

	health = 5;

	return true;
}

bool Player::Update(float dt)
{
	Move_right.speed = animSpeed * dt;
	Move_left.speed = animSpeed * dt;
	Idle_right.speed = animSpeed * dt;
	Idle_left.speed = animSpeed * dt;
	Jump_right.speed = animSpeed * dt;
	Jump_left.speed = animSpeed * dt;
	Die.speed = 0.01f * dt;

	if (app->map->level == 1 &&	position.x > 1930 && position.y < 260 && !win) {
		win = true;
		app->audio->PlayFx(won);
		//pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
		//app->entityManager->DestroyEntity(this);

	}

	if (intoxication > 3) {
		isAlive = false;
		intoxication = 0;
	}

	if(isAlive)
	{ 
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
			itemPicked -= 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
			godmode = !godmode;
		}

		if (falling) {
			countF = (countF + 1);
			pbody->body->GetFixtureList()->SetSensor(true); // Disable collisions
		}

		if (countF >= 40.0f) {
			falling = false;
		}

		b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo

		if (app->scene->tp1) {
			vel.y = 0.2f;
			app->scene->tp1 = false;
		}

		if (app->scene->tp2) {
			vel.y = 0.2f;
			app->scene->tp2 = false;
		}

		if (godmode) {
			vel.y = 0;
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && !jumping) {
				vel.y = -speed * dt; // Moverse hacia arriba
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !jumping) {
				vel.y = speed * dt; // Moverse hacia arriba
			}
			pbody->body->GetFixtureList()->SetSensor(true); // Disable collisions
		}

		if (currentDirection == Direction::RIGHT || currentDirection == Direction::JUMP_R && !jumping)
		{
			currentDirection = Direction::IDLE_R;
		}
		else if (currentDirection == Direction::LEFT || currentDirection == Direction::JUMP_L && !jumping)
		{
			currentDirection = Direction::IDLE_L;
		}

		// Restablecer la velocidad en X para evitar movimientos diagonales no deseados
		vel.x = 0;

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !fell) {
			vel.x = -speed * dt; // Moverse a la izquierda
			currentDirection = Direction::LEFT;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !fell) {
			vel.x = speed * dt; // Moverse a la derecha
			currentDirection = Direction::RIGHT;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !fell && !godmode && !falling && !jumping && vel.y<0.5f && vel.y>-0.5f && collidingPlat) {
			countF = 0.0f;
			falling = true;
		}
		
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && isAlive) {  
			if (cooldown <= 0 && canShoot) //codigo para disparar
			{ 

				for (int i = 0; i <= 1; i++) //dos disparos a la vez
				{
					if (currentAnimation == &Idle_right || currentAnimation == &Move_right || currentAnimation == &Jump_right) {
						app->scene->particulas->Shoot(true, position.x + 32 - (i * 15), position.y + 15 + (i * 5), 1, ColliderType::PLAYER_SHOT);
						app->audio->PlayFx(shoot);
					} 
					else if (currentAnimation == &Idle_left || currentAnimation == &Move_left || currentAnimation == &Jump_left) {
						app->scene->particulas->Shoot(true, position.x + (i * 15), position.y + 11 + (i * 9), -1, ColliderType::PLAYER_SHOT);
						app->audio->PlayFx(shoot);
					}
				}
				isShooting = true;
				canShoot = false;
				timer.Start();
				/*cooldown = 10.0f;*/
			}
		}

		if (isShooting) 
		{
			/*cooldown -= 0.008f * dt;*/
			if (timer.ReadSec() == 1 && isReloading == false)
			{
				app->audio->PlayFx(reload);
				LOG("RELOADING");
				isReloading = true;
			}
			if (timer.ReadSec() == 2)
			{
				canShoot = true;
				isShooting = false;
				isReloading = false;
			}
		}
		
		// Aplicar la velocidad al cuerpo del jugador solo si no est� saltando
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !fell && !jumping && !godmode && !falling && vel.y<0.5f && vel.y>-0.5f) {
			app->audio->PlayFx(jump);
			pbody->body->GetFixtureList()->SetSensor(true); // Disable collisions
			jumping = true;
			vel.y = -10.0f; // Aplicar impulso vertical al saltar

		}
		if (position.y >= 786 && !fell && !godmode) {
			app->audio->PlayFx(fall,60);
			fell = true;
		}
		if(fell){
			if (app->audio->FxEnd(fall)) {
				/*if (app->scene->checkpoint == 0) pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
				else if (app->scene->checkpoint == 1) pbody->body->SetTransform({ PIXEL_TO_METERS(980),PIXEL_TO_METERS(150) }, 0);*/
				app->LoadRequest();
				fell=false;
			}
			
		}
		
		if (!jumping && !falling && !godmode) {
			pbody->body->GetFixtureList()->SetSensor(false); // Enable collisions
		}

		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		if (jumping || (vel.y > 0.05f) || falling)
		{
			if (currentDirection == Direction::RIGHT || currentDirection == Direction::IDLE_R)
			{
				currentDirection = Direction::JUMP_R;
			}
			else if (currentDirection == Direction::LEFT || currentDirection == Direction::IDLE_L)
			{
				currentDirection = Direction::JUMP_L;
			}
		}
		switch (currentDirection)
		{
		case Direction::IDLE_R:
			currentAnimation = &Idle_right;
			break;
		case Direction::IDLE_L:
			currentAnimation = &Idle_left;
			break;
		case Direction::LEFT:
			currentAnimation = &Move_left;
			break;
		case Direction::RIGHT:
			currentAnimation = &Move_right;
			break;
		case Direction::JUMP_R:
			currentAnimation = &Jump_right;
			break;
		case Direction::JUMP_L:
			currentAnimation = &Jump_left;
			break;

		}
		
		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}
	else
	{
		//en caso de que muera se desactivan las colisiones y se activa la animacion de muerte
		pbody->body->GetFixtureList()->SetSensor(true);
		if (currentAnimation->GetCurrentFrameIndex() >= 0) app->audio->PlayFx(blood,24);
		currentAnimation = &Die;
		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);

		//si la animacion de muerte termina se resetea la posicion del jugador y se activa la animacion de idle
		if (currentAnimation->GetCurrentFrameIndex() >= 7)
		{
			app->scene->player->isAlive = true;
			app->scene->player->currentAnimation->Reset();

			app->LoadRequest();
			/*if (app->scene->checkpoint == 0) pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
			else if (app->scene->checkpoint == 1) pbody->body->SetTransform({ PIXEL_TO_METERS(980),PIXEL_TO_METERS(150) }, 0);*/
			
			//intoxication = 0;
		}
	}
	return true;
}


bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo
	
	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(sniff);
		if (vel.y > 0)jumping = false;
		if(isAlive) state::PICKED_TRUE;
		collidingPlat = false;
		if (itemPicked <= 2)
		{
			itemPicked += 1;
		}
		//intoxication += 1;
		break;
	case ColliderType::PLATFORM:
		//if(!jumping)pbody->body->GetFixtureList()->SetSensor(false); // Enable collisions
		if (vel.y > 0)jumping = false;

		collidingPlat = true;
		LOG("Collision PLATFORM");
		break;
	case ColliderType::WALL:
		if(!godmode && isAlive) pbody->body->GetFixtureList()->SetSensor(false); // Enable collisions
		LOG("Collision WALL");
		break;
	case ColliderType::FLOOR:
		jumping = false;
		falling = false;
		if(!godmode&&isAlive)pbody->body->GetFixtureList()->SetSensor(false); // Enable collisions

		collidingPlat = false;
		LOG("Collision FLOOR");
		break;
	case ColliderType::SPIKES:
		if (vel.y > 0)jumping = false;
		falling = false;
		if (!godmode) {
			isAlive = false;
			health = 0;
		}
		LOG("Collision SPIKES");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::ENEMY_SHOT:
		if (vel.y > 0)jumping = false;
		falling = false;
		if (!godmode)
		{
			if (health <= 0)
			{
				isAlive = false;
			}
			else
			{
				health -= 1;
			}
			LOG("HEALTH: %d", health);
		}
		LOG("Collision SHOT");
		break;
	case ColliderType::ENEMY:
		if (vel.y > 0)jumping = false;
		falling = false;
		if (!godmode)
		{
			if (health <= 0)
			{
				isAlive = false;
			}
			else
			{
				health -= 1;
			}
			LOG("HEALTH: %d", health);
		}
		LOG("Collision ENEMY");
		break;
	case ColliderType::BOSS:
		if (vel.y > 0)jumping = false;
		falling = false;
		if (!godmode)
		{
			if (health <= 0)
			{
				isAlive = false;
			}
			else
			{
				health -= 4;
			}
			LOG("HEALTH: %d", health);
		}
		LOG("Collision BOSS");
		break;
	case ColliderType::HEAL:
		LOG("Collision HEAL");
		//app->audio->PlayFx(sniff);
		if (vel.y > 0)jumping = false;
		if (health < 5)
		{
			health += 1;
		}
		
		LOG("HEALTH: %d", health);
		collidingPlat = false;
		break;
	}
}