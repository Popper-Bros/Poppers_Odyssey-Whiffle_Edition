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

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 12, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	sniff = app->audio->LoadFx("Assets/Audio/Fx/sniff.ogg");

	Move_derecha[0] = { 14, 16, 29, 31 };
	Move_derecha[1] = { 79,18,25,31 };
	Move_derecha[2] = { 147,20,22,29 };
	Move_derecha[3] = { 212,17,22,32 };
	Move_derecha[4] = { 269,16,31,30 };
	Move_derecha[5] = { 334,18,28,31 };
	Move_derecha[6] = { 404,20,21,29 };
	Move_derecha[7] = { 469,17,20,32 };

	Move_izquierda[0] = { 471, 80, 29, 31 };
	Move_izquierda[1] = { 410,82,25,31 };
	Move_izquierda[2] = { 345,84,22,29 };
	Move_izquierda[3] = { 280,81,22,32 };
	Move_izquierda[4] = { 214,80,31,30 };
	Move_izquierda[5] = { 152,82,28,31 };
	Move_izquierda[6] = { 89,84,21,29 };
	Move_izquierda[7] = { 25,81,20,32 };

	Idle_right[0] = { 15, 149, 26, 28 };
	Idle_right[1] = { 78, 148, 26, 29 };
	Idle_right[2] = { 143, 147, 25, 30 };
	Idle_right[3] = { 143, 147, 25, 30 };
	Idle_right[4] = { 206, 147, 28, 30 };
	Idle_right[5] = { 271, 147, 26, 30 };
	Idle_right[6] = { 271, 147, 26, 30 };
	Idle_right[7] = { 335, 149, 26, 28 };

	Idle_left[0] = { 345, 197, 26, 28 };
	Idle_left[1] = { 281, 196, 26, 29 };
	Idle_left[2] = { 218, 195, 25, 30 };
	Idle_left[3] = { 218, 195, 25, 30 };
	Idle_left[4] = { 152, 195, 28, 30 };
	Idle_left[5] = { 89, 195, 26, 30 };
	Idle_left[6] = { 89, 195, 26, 30 };
	Idle_left[7] = { 25, 197, 26, 28 };

	Jump_left[0] = { 252,271,28,35 };
	Jump_left[1] = { 252,271,28,35 };
	Jump_left[2] = { 252,271,28,35 };
	Jump_left[3] = { 252,271,28,35 };
	Jump_left[4] = { 252,271,28,35 };
	Jump_left[5] = { 252,271,28,35 };
	Jump_left[6] = { 252,271,28,35 };
	Jump_left[7] = { 252,271,28,35 };

	Jump_right[0] = { 399,236,30,36 };
	Jump_right[1] = { 399,236,30,36 };
	Jump_right[2] = { 399,236,30,36 };
	Jump_right[3] = { 399,236,30,36 };
	Jump_right[4] = { 399,236,30,36 };
	Jump_right[5] = { 399,236,30,36 };
	Jump_right[6] = { 399,236,30,36 };
	Jump_right[7] = { 399,236,30,36 };

	return true;
}

bool Player::Update(float dt)
{

	if (falling) {
		countF = (countF + 1);
		pbody->body->GetFixtureList()->SetSensor(true); // Disable collisions
	}

	if (countF >= 40.0f) {
		falling = false;
	}

	b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_REPEAT) {
		if (godmode) {
			godmode = false;
		}
		else {
			godmode = true;
		}
	}
	if (!godmode) {

		if (currentDirection == Direction::RIGHT || currentDirection == Direction::JUMP_R && !jumping)
		{
			currentDirection = Direction::IDLE_R;
		}
		else if (currentDirection == Direction::LEFT || currentDirection == Direction::JUMP_L && !jumping)
		{
			currentDirection = Direction::IDLE_L;
		}

		frameCounter += frameSpeed;

		if (frameCounter >= NUM_FRAMES) {
			frameCounter = 0.0f;
			currentFrame++;

			if (currentFrame >= NUM_FRAMES) {
				currentFrame = 0;
			}
		}
		// Restablecer la velocidad en X para evitar movimientos diagonales no deseados
		vel.x = 0;

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel.x = -speed * dt; // Moverse a la izquierda

			currentDirection = Direction::LEFT;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel.x = speed * dt; // Moverse a la derecha
			currentDirection = Direction::RIGHT;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !falling && !jumping && vel.y<0.5f && vel.y>-0.5f && collidingPlat) {
			countF = 0.0f;
			falling = true;
		}

		// Aplicar la velocidad al cuerpo del jugador solo si no está saltando
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !jumping && !falling && vel.y<0.5f && vel.y>-0.5f) {
			pbody->body->GetFixtureList()->SetSensor(true); // Disable collisions
			jumping = true;
			vel.y = -10.0f; // Aplicar impulso vertical al saltar
		}


		if (!jumping && !falling) {
			pbody->body->GetFixtureList()->SetSensor(false); // Enable collisions
		}

		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		if (jumping||(vel.y<-0.05f && vel.y>0.05f)||falling)
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
			currentAnimation = Idle_right;
			break;
		case Direction::IDLE_L:
			currentAnimation = Idle_left;
			break;
		case Direction::LEFT:
			currentAnimation = Move_izquierda;
			break;
		case Direction::RIGHT:
			currentAnimation = Move_derecha;
			break;
		case Direction::JUMP_R:
			currentAnimation = Jump_right;
			break;
		case Direction::JUMP_L:
			currentAnimation = Jump_left;
			break;
		}

		if (currentAnimation != nullptr) {
			destRect = currentAnimation[currentFrame];
			app->render->DrawTexture(texture, position.x, position.y, &destRect);
		}
	}
	else {

		pbody->body->GetFixtureList()->SetSensor(true);

		if (currentDirection == Direction::RIGHT || currentDirection == Direction::JUMP_R && !jumping)
		{
			currentDirection = Direction::IDLE_R;
		}
		else if (currentDirection == Direction::LEFT || currentDirection == Direction::JUMP_L && !jumping)
		{
			currentDirection = Direction::IDLE_L;
		}

		frameCounter += frameSpeed;

		if (frameCounter >= NUM_FRAMES) {
			frameCounter = 0.0f;
			currentFrame++;

			if (currentFrame >= NUM_FRAMES) {
				currentFrame = 0;
			}
		}
		// Restablecer la velocidad en X para evitar movimientos diagonales no deseados
		vel.x = 0;

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel.x = -speed * dt; // Moverse a la izquierda

			currentDirection = Direction::LEFT;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel.x = speed * dt; // Moverse a la derecha

			currentDirection = Direction::RIGHT;
		}
		// Aplicar la velocidad al cuerpo del jugador solo si no está saltando
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			vel.y = speed * dt; // Moverse a la derecha
			if (currentDirection == Direction::RIGHT || currentDirection == Direction::IDLE_R)
			{
				currentDirection = Direction::JUMP_R;
			}
			else if (currentDirection == Direction::LEFT || currentDirection == Direction::IDLE_L)
			{
				currentDirection = Direction::JUMP_L;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			vel.y = -speed * dt; // Moverse a la derecha
			if (currentDirection == Direction::RIGHT || currentDirection == Direction::IDLE_R)
			{
				currentDirection = Direction::JUMP_R;
			}
			else if (currentDirection == Direction::LEFT || currentDirection == Direction::IDLE_L)
			{
				currentDirection = Direction::JUMP_L;
			}
		}

		pbody->body->SetLinearVelocity(vel);

		//Update player position in pixels
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		switch (currentDirection)
		{
		case Direction::IDLE_R:
			currentAnimation = Idle_right;
			break;
		case Direction::IDLE_L:
			currentAnimation = Idle_left;
			break;
		case Direction::LEFT:
			currentAnimation = Move_izquierda;
			break;
		case Direction::RIGHT:
			currentAnimation = Move_derecha;
			break;
		case Direction::JUMP_R:
			currentAnimation = Jump_right;
			break;
		case Direction::JUMP_L:
			currentAnimation = Jump_left;
			break;
		}


		if (currentAnimation != nullptr) {
			destRect = currentAnimation[currentFrame];
			app->render->DrawTexture(texture, position.x, position.y, &destRect);
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

		collidingPlat = false;
		break;
	case ColliderType::PLATFORM:
		//if(!jumping)pbody->body->GetFixtureList()->SetSensor(false); // Enable collisions
		if (vel.y > 0)jumping = false;

		collidingPlat = true;
		LOG("Collision PLATFORM");
		break;
	case ColliderType::WALL:
		pbody->body->GetFixtureList()->SetSensor(false); // Enable collisions
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::FLOOR:
		if (vel.y > 0)jumping = false;
		falling = false;

		collidingPlat = false;
		LOG("Collision FLOOR");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}