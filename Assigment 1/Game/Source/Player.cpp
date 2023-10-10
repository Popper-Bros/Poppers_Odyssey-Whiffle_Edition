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
	texturePath_M_D = parameters.attribute("texturepathM_D").as_string();
	texturePath_M_I = parameters.attribute("texturepathM_I").as_string();

	

	return true;
}

bool Player::Start() {

	//initilize textures
	texture_Move_Derecha = app->tex->Load(texturePath_M_D);
	texture_Move_Izquierda = app->tex->Load(texturePath_M_I);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	Move_derecha[0] = { 14, 16, 29, 31 };
	Move_derecha[1] = { 79,18,25,31 };
	Move_derecha[2] = { 147,20,22,29 };
	Move_derecha[3] = { 212,17,22,32 };
	Move_derecha[4] = { 269,16,31,30 };
	Move_derecha[5] = { 334,18,28,31 };
	Move_derecha[6] = { 404,20,21,29 };
	Move_derecha[7] = { 469,17,20,32 };

	Move_izquierda[0] = { 14, 16, 29, 31 };
	Move_izquierda[1] = { 79,18,25,31 };
	Move_izquierda[2] = { 147,20,22,29 };
	Move_izquierda[3] = { 212,17,22,32 };
	Move_izquierda[4] = { 269,16,31,30 };
	Move_izquierda[5] = { 334,18,28,31 };
	Move_izquierda[6] = { 404,20,21,29 };
	Move_izquierda[7] = { 469,17,20,32 };

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = pbody->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo
	

	// Restablecer la velocidad en X para evitar movimientos diagonales no deseados
	vel.x = 0;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !jumping) {
		jumping = true;
		vel.y = -10.0f; // Aplicar impulso vertical al saltar
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel.x = -speed*dt; // Moverse a la izquierda

		currentFrame = Move_Derecha(currentFrame, frameCounter, frameSpeed);
		SDL_Rect Iz = Move_izquierda[currentFrame];
		app->render->DrawTexture(texture_Move_Izquierda, position.x, position.y, &Iz);
	}
	

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel.x = speed*dt; // Moverse a la derecha

		currentFrame = Move_Derecha(currentFrame, frameCounter, frameSpeed);
		
	}
	destRect = Move_derecha[currentFrame];
	// Aplicar la velocidad al cuerpo del jugador solo si no está saltando
	if (!jumping) {
		vel.y = -GRAVITY_Y; // Aplicar gravedad en el eje Y
	}

	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	
	app->render->DrawTexture(texture_Move_Derecha, position.x, position.y, &destRect);
	

	
	return true;
}
int Player::Move_Derecha(int &currentF, float &frameC, const int frameS)
{
	frameC += frameS;

	if (frameC >= NUM_FRAMES) 
	{
		frameC = 0;
		currentF++;

		if (currentF >= NUM_FRAMES) 
		{
			currentF = 0;
		}
	}
	return currentF;

}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		jumping = false;
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}