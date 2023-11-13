#include "Enemy.h"
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




Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Enemy");
}

Enemy::~Enemy() {}

bool Enemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePathEnemy = parameters.attribute("enemyTexturepath").as_string();

	return true;
}

bool Enemy::Start() {

	//initilize textures
	texture = app->tex->Load(texturePathEnemy);

	pbody = app->physics->CreateCircle(position.x, position.y + 16, 9, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

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

bool Enemy::Update(float dt)
{
	Move_right.speed = 0.01f * dt;
	Move_left.speed = 0.01f * dt;
	Idle_right.speed = 0.01f * dt;
	Idle_left.speed = 0.01f * dt;
	Jump_right.speed = 0.01f * dt;
	Jump_left.speed = 0.01f * dt;
	Die.speed = 0.01f * dt;
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	switch (currentDirection)
	{
	case EnemyDirection::IDLE_R:
		currentAnimation = &Idle_right;
		break;
	case EnemyDirection::IDLE_L:
		currentAnimation = &Idle_left;
		break;
	case EnemyDirection::LEFT:
		currentAnimation = &Move_left;
		break;
	case EnemyDirection::RIGHT:
		currentAnimation = &Move_right;
		break;
	case EnemyDirection::JUMP_R:
		currentAnimation = &Jump_right;
		break;
	case EnemyDirection::JUMP_L:
		currentAnimation = &Jump_left;
		break;

	}

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

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