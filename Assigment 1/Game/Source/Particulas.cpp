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
	name.Create("item");
}

Particulas::~Particulas() {}

bool Particulas::Awake() {

	balas.Clear();
	//position.x = parameters.attribute("x").as_int();
	//position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("particulasTexturepath").as_string();

	return true;
}

bool Particulas::Start() {

	

	
	//initilize textures
	/*bala = app->physics->CreateRectangle(position.x + 32, position.y + 16, rec.w, rec.h, bodyType::KINEMATIC);
	bala->listener = this;
	bala->ctype = ColliderType::SHOT;
	bala->body->IsBullet();
	bala->body->SetLinearVelocity(b2Vec2(2, 0));*/

	

	texture = app->tex->Load(texturePath);

	return true;
}

bool Particulas::Update(float dt)
{


	for (ListItem<BalaInfo>* item = balas.start; item; item = item->next)
	{
		if (item->data.balaBody != nullptr && item->data.balaBody->body != nullptr)
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
			app->render->DrawTexture(texture, item->data.posx, item->data.posy, &rec);
			item->data.currentShotAnim->Update();



			if (item->data.life < 1 || item->data.collision == true )
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

void Particulas::Shoot(bool disparar, int positionX, int positionY)
{
	
	
	PhysBody* bala = app->physics->CreateRectangle(positionX + 32, positionY + 16, 15, 15, bodyType::DYNAMIC);
	LOG("!!CREATED!!");
	bala->listener = this;
	bala->ctype = ColliderType::SHOT;
	bala->body->GetFixtureList()->SetDensity(0.1f);
	bala->body->SetGravityScale(0.1f); // Reducir la influencia de la gravedad
	b2Vec2 initialVelocity(5.0f, 0.1f); // Ajustar la velocidad inicial
	bala->body->SetLinearVelocity(initialVelocity);
	

	BalaInfo nuevaBala(bala, Animation(), Animation(), 150, 1, 0, 0, false);
		
	nuevaBala.shot.PushBack({ 15,8,29,20 });
	nuevaBala.shot.PushBack({ 66,8,32,20 });
	nuevaBala.shot.PushBack({ 121,8,36,20 });
	nuevaBala.shot.PushBack({ 175,8,36,22 });
	nuevaBala.shot.PushBack({ 228,8,34,20 });
	nuevaBala.shot.PushBack({ 279,8,36,20 });
	nuevaBala.shot.PushBack({ 328,8,36,22 });
	nuevaBala.shot.loop = true;
	nuevaBala.shot.pingpong = false;

	nuevaBala.endShot.PushBack({ 381,6,37,24 });
	nuevaBala.endShot.PushBack({ 436,8,48,40 });
	nuevaBala.endShot.PushBack({ 501,12,46,40 });
	nuevaBala.endShot.pingpong = false;
	nuevaBala.endShot.loop = false;

	nuevaBala.currentShotAnim = &nuevaBala.shot;
	nuevaBala.currentShotAnim->Reset();

	nuevaBala.collision = false;

	balas.Add(nuevaBala);
	contador++;

	LOG("N�mero de elementos en la lista de balas: %d", balas.Count());
	

}


void Particulas::OnCollision(PhysBody* physA, PhysBody* physB)
{

	for (ListItem<BalaInfo>* item = balas.start; item; item = item->next)
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
			case ColliderType::SHOT:

				LOG("BALA COLISION PLATFORM");
				item->data.collision = true;
				break;
			}
			
		}
		
		
	}
	
	
}

bool Particulas::CleanUp()
{
	for (ListItem<BalaInfo>* item = balas.start; item != nullptr; )
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