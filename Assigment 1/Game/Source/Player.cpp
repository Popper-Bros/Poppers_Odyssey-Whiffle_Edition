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

	Move_right.PushBack({ 11, 15, 32, 34 });
	Move_right.PushBack({ 72,17,32, 34 });
	Move_right.PushBack({ 137,17,32, 34 });
	Move_right.PushBack({ 202,17,32, 34 });
	Move_right.PushBack({ 268,14,32, 34 });
	Move_right.PushBack({ 330,17,32, 34 });
	Move_right.PushBack({ 393,17,32, 34 });
	Move_right.PushBack({ 457,17,32, 34 });
	

	Move_left.PushBack({ 468, 79, 32, 34 });
	Move_left.PushBack({ 403,81,32, 34 });
	Move_left.PushBack({ 335,81,32, 34 });
	Move_left.PushBack({ 270,81,32, 34 });
	Move_left.PushBack({ 213,78 ,32, 34 });
	Move_left.PushBack({ 148,81,32, 34 });
	Move_left.PushBack({ 78,81,32, 34 });
	Move_left.PushBack({ 13,81,32, 34 });

	Idle_right.PushBack({ 12, 148, 29, 31 });
	Idle_right.PushBack({ 77, 148, 29, 31 });
	Idle_right.PushBack({ 140, 148, 29, 31 });
	Idle_right.PushBack({ 269, 148, 29, 31 });
	Idle_right.PushBack({ 206, 148, 29, 31 });
	Idle_right.PushBack({ 269, 148, 29, 31 });
	Idle_right.PushBack({ 333, 148, 29, 31 });

	Idle_left.PushBack({ 343, 196, 29, 31 });
	Idle_left.PushBack({ 279, 196, 29, 31 });
	Idle_left.PushBack({ 215, 196, 29, 31 });
	Idle_left.PushBack({ 89, 196, 29, 31 });
	Idle_left.PushBack({ 152, 196, 29, 31 });
	Idle_left.PushBack({ 87, 196, 29, 31 });
	Idle_left.PushBack({ 23, 196, 29, 31 });

	Jump_left.PushBack({ 252,271,28,35 });
	Jump_left.PushBack({ 252,271,28,35 });
	Jump_left.PushBack({ 252,271,28,35 });
	Jump_left.PushBack({ 252,271,28,35 });
	Jump_left.PushBack({ 252,271,28,35 });
	Jump_left.PushBack({ 252,271,28,35 });
	Jump_left.PushBack({ 252,271,28,35 });
	Jump_left.PushBack({ 252,271,28,35 });

	Jump_right.PushBack({ 399,236,30,36 });
	Jump_right.PushBack({ 399,236,30,36 });
	Jump_right.PushBack({ 399,236,30,36 });
	Jump_right.PushBack({ 399,236,30,36 });
	Jump_right.PushBack({ 399,236,30,36 });
	Jump_right.PushBack({ 399,236,30,36 });
	Jump_right.PushBack({ 399,236,30,36 });
	Jump_right.PushBack({ 399,236,30,36 });

	Die.PushBack({ 11, 315, 48, 51 });
	Die.PushBack({ 72, 316, 48, 51 });
	Die.PushBack({ 134, 318, 48, 51 });
	Die.PushBack({ 196, 317, 48, 51 });
	Die.PushBack({ 260, 320, 48, 51 });
	Die.PushBack({ 325, 322, 48, 51 });
	Die.PushBack({ 390, 325, 48, 51 });
	Die.PushBack({ 454, 326, 48, 51 });
	Die.loop = false;


	return true;
}

bool Player::Update(float dt)
{
	Move_right.speed = 0.01f * dt;
	Move_left.speed = 0.01f * dt;
	Idle_right.speed = 0.01f * dt;
	Idle_left.speed = 0.01f * dt;
	Jump_right.speed = 0.01f * dt;
	Jump_left.speed = 0.01f * dt;
	Die.speed = 0.01f * dt;
	if (position.x > 1930 && position.y < 260) {
		win = true;
		app->scene->checkpoint = 0;
		pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
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
	}
}