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
#include "Enemy.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("enemy");
}

Enemy::~Enemy() {}

bool Enemy::Awake() {

	/*position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();*/

	return true;
}

bool Enemy::Start() {

	////initilize textures
	//texture = app->tex->Load(texturePath);
	//pbody = app->physics->CreateRectangle(position.x, position.y, 12, 16, bodyType::DYNAMIC);
	//pbody->listener = this;
	//pbody->ctype = ColliderType::ITEM;

	//return true;
	return true;
}

bool Enemy::Update(float dt)
{
	//// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	//position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
	//position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

	//app->render->DrawTexture(texture, position.x, position.y);

	//if (isPicked)
	//{
	//	Disable();
	//	app->physics->ChupaBody(app->physics->GetWorld(), pbody->body);
	//}
	//return true;
	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB)
{
	//switch (physB->ctype)
	//{
	//case ColliderType::PLAYER:
	//	LOG("Collision ITEM con PLAYER");
	//	if(app->scene->playerAlive) isPicked = true;
	//	break;
	//}
		
}

bool Enemy::CleanUp()
{
	return true;
}