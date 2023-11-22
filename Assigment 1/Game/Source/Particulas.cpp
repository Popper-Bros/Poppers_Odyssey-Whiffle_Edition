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

	shot.PushBack({ 15,8,29,20 });
	shot.PushBack({ 66,8,32,20 });
	shot.PushBack({ 121,8,36,20 });
	shot.PushBack({ 175,8,36,22 });
	shot.PushBack({ 228,8,34,20 });
	shot.PushBack({ 279,8,36,20 });
	shot.PushBack({ 328,8,36,22 });

	shot.loop = true;
	shot.pingpong = true;
	shot.speed = 0.1f;

	endShot.PushBack({ 381,6,37,24 });
	endShot.PushBack({ 436,8,48,40 });
	endShot.PushBack({ 501,12,46,40 });

	texture = app->tex->Load(texturePath);

	return true;
}

bool Particulas::Update(float dt)
{
	if (bala != nullptr)
	{
		balaposx = METERS_TO_PIXELS(bala->body->GetTransform().p.x);
		balaposy = METERS_TO_PIXELS(bala->body->GetTransform().p.y);
	}
	

	

	/*switch (currentState)
	{
	case shooting::ALIVE:
		currentShotAnim = &shot;
		break;
	case shooting::DEAD:
		currentShotAnim = &endShot;
		break;
	}*/
	if (bala != nullptr)
	{
		bulletlife--;
		rec = currentShotAnim->GetCurrentFrame();
		app->render->DrawTexture(texture, balaposx, balaposy, &rec);
		currentShotAnim->Update();
	}
	
	return true;
}

void Particulas::Shoot(bool disparar, int positionX, int positionY)
{
	if (disparar == true && contador <= 1)
	{
		bala = app->physics->CreateRectangle(positionX + 32, positionY + 16, rec.w, rec.h, bodyType::KINEMATIC);
		bala->listener = this;
		bala->ctype = ColliderType::SHOT;
		bala->body->IsBullet();
		bala->body->SetLinearVelocity(b2Vec2(2, 0));
		contador++;
	}
	if (bala != nullptr && bulletlife == 0)
	{
		app->physics->ChupaBody(app->physics->GetWorld(),bala->body);
		bulletlife = 150;
	}
	if (disparar == false && bala != nullptr && onCollision == false)
	{
		currentShotAnim = &endShot;
		app->render->DrawTexture(texture, balaposx - 16, balaposy - 12, &rec);
		this->Disable();
		//disparar = false;
		//bulletlife = 150;
		contador = 0;		
	}
}

void Particulas::unShot()
{
	currentShotAnim = &endShot;
	app->render->DrawTexture(texture, balaposx - 16, balaposy - 12, &rec);
	currentShotAnim->Update();
	onCollision = false;
}

void Particulas::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::WALL:
		unShot();
		onCollision = true;
		break;
	}
	
}

bool Particulas::CleanUp()
{
	return true;
}