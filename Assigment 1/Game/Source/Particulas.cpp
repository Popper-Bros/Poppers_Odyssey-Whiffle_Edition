#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Particulas::Particulas() : Entity(EntityType::PARTICULAS)
{
	name.Create("Particulas");
}

Particulas::~Particulas() {}

bool Particulas::Awake() {

	balas.Clear();
	//position.x = parameters.attribute("x").as_int();
	//position.y = parameters.attribute("y").as_int();
	texturepath = parameters.attribute("texturepath").as_string();

	playerShot.LoadAnimation("Particulas", "playerShot");
	playerShotFinal.LoadAnimation("Particulas", "playerShotFinal");
	enemyShadowShot.LoadAnimation("Particulas", "enemyShadowShot");
	enemyShadowShotFinal.LoadAnimation("Particulas", "enemyShadowShotFinal");
	enemyShadowShotLeft.LoadAnimation("Particulas", "enemyShadowShotLeft");
	enemyShadowShotFinalLeft.LoadAnimation("Particulas", "enemyShadowShotFinalLeft");
	return true;
}

bool Particulas::Start() {


	texture = app->tex->Load(texturepath);

	return true;
}



bool Particulas::Update(float dt) 
{
	this->dt = dt;
	for (ListItem<BalaInfo>* item = balas.start; item; item = item->next) // Recorre la lista de balas
	{
		if (item->data.balaBody != nullptr && item->data.balaBody->body != nullptr) // Si la bala existe y su cuerpo existe
		{
			item->data.shot.speed = 0.01f * dt;
			item->data.endShot.speed = 0.007f * dt;
			item->data.alive = true;
			

			item->data.posx = METERS_TO_PIXELS(item->data.balaBody->body->GetTransform().p.x);
			item->data.posy = METERS_TO_PIXELS(item->data.balaBody->body->GetTransform().p.y);

			
			while (item->data.bucle >=1)
			{
				item->data.currentShotAnim = &item->data.shot;
				item->data.bucle--;
			}
			SDL_Rect rec;
			rec = item->data.currentShotAnim->GetCurrentFrame();
			int a;
			int b;
			if (item->data.balaBody->ctype == ColliderType::PLAYER_SHOT)
			{
				a = 5;
				b = 5;
			}
			else if (item->data.balaBody->ctype == ColliderType::ENEMY_SHOT)
			{
				a = 10;
				b = 10;
			}
			app->render->DrawTexture(texture, item->data.posx-a, item->data.posy-b, &rec);
			item->data.currentShotAnim->Update();



			if (item->data.life < 1 || item->data.collision == true ) // Si la bala ha colisionado o ha llegado al final de su vida
			{

				item->data.currentShotAnim = &item->data.endShot;
				item->data.balaBody->body->SetLinearVelocity(b2Vec2_zero);
				if (item->data.currentShotAnim == &item->data.endShot && item->data.currentShotAnim->HasFinished()) 
				{
					item->data.alive = false;
					CleanUp();
				}
			}
			item->data.life--;
		}
	}
	
	return true;
}

void Particulas::Shoot(bool disparar, int positionX, int positionY, int direction, ColliderType coll) // Disparar la bala en la posicion x, y con el tipo de bala y la direccion
{
	int width;
	int height;
	int a;
	int b;
	//diferentes casos para el tamaño de la bala
	if (coll == ColliderType::PLAYER_SHOT)
	{
		width = 5;
		height = 5;
	}
	else if (coll == ColliderType::ENEMY_SHOT)
	{
		width = 20;
		height = 20;
	}
	PhysBody* bala = app->physics->CreateRectangle(positionX, positionY, width, height, bodyType::DYNAMIC);
	LOG("!!CREATED!!");
	bala->listener = this;
	bala->ctype = coll;
	bala->body->GetFixtureList()->SetDensity(0.01f);
	bala->body->SetGravityScale(0.0f); // Reducir la influencia de la gravedad

	b2Vec2 vel = bala->body->GetLinearVelocity(); // Obtener la velocidad actual del cuerpo
	
	if (direction == 1) {
		vel.x = 0.4f * dt; // Ajustar la velocidad inicial a la derecha

	}
	else if (direction == -1) {
		vel.x = -0.4f * dt; // Ajustar la velocidad inicial a la izuqierda

	}
	bala->body->SetLinearVelocity(vel);
	
	
	BalaInfo nuevaBala(bala, Animation(), Animation(), 150, 1, 0, 0, false);
		
	switch (coll)
	{
	case ColliderType::PLAYER_SHOT:
		nuevaBala.shot = playerShot;
		nuevaBala.endShot = playerShotFinal;
		break;
	case ColliderType::ENEMY_SHOT: // Cambiar la animacion de la bala dependiendo de su tipo
		
		if (direction == 1)
		{
			nuevaBala.shot = enemyShadowShot;
			nuevaBala.endShot = enemyShadowShotFinal;
		}
		else if (direction == -1)
		{
			nuevaBala.shot = enemyShadowShotLeft;
			nuevaBala.endShot = enemyShadowShotFinalLeft;
		}
		break;
	case ColliderType::UNKNOWN:
		break;
	default:
		break;
	}

	nuevaBala.currentShotAnim = &nuevaBala.shot;
	nuevaBala.currentShotAnim->Reset();

	nuevaBala.collision = false;

	balas.Add(nuevaBala);
	contador++;

	LOG("Numero de elementos en la lista de balas: %d", balas.Count());

}


void Particulas::OnCollision(PhysBody* physA, PhysBody* physB) //gestionar las colisiones de las balas
{

	for (ListItem<BalaInfo>* item = balas.start; item; item = item->next) // Recorre la lista de balas
	{
		if (item->data.balaBody != nullptr && item->data.balaBody->body != nullptr && item->data.balaBody == physA)
		{
			switch (physB->ctype)
			{
			case ColliderType::WALL:

				LOG("BALA COLISION WALL");
				item->data.collision = true;
				break;

			case ColliderType::PLATFORM:

				LOG("BALA COLISION PLATFORM");

				item->data.collision = true;

				break;
			case ColliderType::FLOOR:

				LOG("BALA COLISION PLATFORM");
				item->data.collision = true;
				break;
			case ColliderType::SPIKES:

				LOG("BALA COLISION SPIKE");
				item->data.collision = true;
				break;
			case ColliderType::PLAYER_SHOT:

				/*LOG("BALA COLISION SHOT");*/
				item->data.collision = true;
				break;
			case ColliderType::ENEMY:

				LOG("BALA COLISION ENEMY");
				item->data.collision = true;
				break;
			case ColliderType::PLAYER:
				if (physA->ctype == ColliderType::PLAYER_SHOT)
				{
					physA->body->GetFixtureList()->SetSensor(true);
					//item->data.balaBody->body->GetFixtureList()->SetSensor(true);
				}
				
				else
				{
					item->data.collision = true;
				}
				LOG("BALA COLISION PLAYER");
				
				break;
			}
		}
	}
}

bool Particulas::CleanUp()
{
	for (ListItem<BalaInfo>* item = balas.start; item != nullptr; ) // Recorre la lista de balas y elimina las que ya no estan vivas
	{
		if (item->data.balaBody != nullptr && !item->data.alive && item->data.collision)
		{
			app->physics->ChupaBody(app->physics->GetWorld(), item->data.balaBody->body);
			ListItem<BalaInfo>* to_remove = item;
			item = item->next;
			balas.Del(to_remove);
		}
		else
		{
			item = item->next;
		}
	}
	//balas.Clear();
	return true;
}