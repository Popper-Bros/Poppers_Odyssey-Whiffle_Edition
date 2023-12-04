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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("rataTexturepath").as_string();	

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
	texture = app->tex->Load(texturePath);
	

	pbody = app->physics->CreateCircle(position.x, position.y + 16, 9, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	

	sniff = app->audio->LoadFx("Assets/Audio/Fx/sniff.ogg");
	fall = app->audio->LoadFx("Assets/Audio/Fx/fall.ogg");
	jump = app->audio->LoadFx("Assets/Audio/Fx/jump.ogg");
	blood = app->audio->LoadFx("Assets/Audio/Fx/blood.ogg");

	return true;
}

bool Player::Update(float dt)
{
	if (position.x > 1930 && position.y < 260) {
		win = true;
		app->scene->checkpoint = 0;
		pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
	}

	if (intoxication > 2) {
		isAlive = false;
		intoxication = 0;
	}

	if(isAlive)
	{ 
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

		
		// Aplicar la velocidad al cuerpo del jugador solo si no está saltando
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !fell && !jumping && !godmode && !falling && vel.y<0.5f && vel.y>-0.5f) {
			app->audio->PlayFx(jump);
			pbody->body->GetFixtureList()->SetSensor(true); // Disable collisions
			jumping = true;
			vel.y = -0.625f * dt; // Aplicar impulso vertical al saltar
		}
		if (position.y >= 786 && !fell) {
			app->audio->PlayFx(fall,60);
			fell = true;
		}
		if(fell){
			if (app->audio->FxEnd(fall)) {
				if (app->scene->checkpoint == 0) pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
				else if (app->scene->checkpoint == 1) pbody->body->SetTransform({ PIXEL_TO_METERS(980),PIXEL_TO_METERS(150) }, 0);
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
		pbody->body->GetFixtureList()->SetSensor(true);
		if (currentAnimation->GetCurrentFrameIndex() >= 0) app->audio->PlayFx(blood,24);
		currentAnimation = &Die;
		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		
		if (currentAnimation->GetCurrentFrameIndex() >= 7)
		{
			isAlive = true;
			currentAnimation->Reset();
			if (app->scene->checkpoint == 0) pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
			else if (app->scene->checkpoint == 1) pbody->body->SetTransform({ PIXEL_TO_METERS(980),PIXEL_TO_METERS(150) }, 0);
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
		intoxication += 1;
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
		if (!godmode) isAlive = false;
		LOG("Collision SPIKES");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::PLAYER_SHOT:
		if (vel.y > 0)jumping = false;
		falling = false;
		if (!godmode) isAlive = false;
		LOG("Collision SHOT");
		break;

	}
}