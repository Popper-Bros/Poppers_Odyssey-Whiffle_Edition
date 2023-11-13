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

	Move_right.PushBack({ 19+(50*0), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 1), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 2), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 3), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 4), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 5), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 6), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 7), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 8), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 9), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 10), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 11), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 12), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 13), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 14), 16, 48, 48 });
	Move_right.PushBack({ 19 + (50 * 15), 16, 48, 48 });

	Idle_right.PushBack({ 19 + (50 * 0), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 1), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 2), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 3), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 4), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 5), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 6), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 7), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 8), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 9), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 10), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 11), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 12), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 13), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 14), 16, 48, 48 });
	Idle_right.PushBack({ 19 + (50 * 15), 16, 48, 48 });

	Move_left.PushBack({ 19 + (50 * 0), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 1), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 2), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 3), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 4), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 5), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 6), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 7), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 8), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 9), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 10), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 11), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 12), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 13), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 14), 66, 48, 48 });
	Move_left.PushBack({ 19 + (50 * 15), 66, 48, 48 });

	Idle_left.PushBack({ 19 + (50 * 0), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 1), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 2), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 3), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 4), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 5), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 6), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 7), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 8), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 9), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 10), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 11), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 12), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 13), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 14), 66, 48, 48 });
	Idle_left.PushBack({ 19 + (50 * 15), 66, 48, 48 });


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
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 22;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 28;


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