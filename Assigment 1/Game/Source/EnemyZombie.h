#ifndef __EnemyZombie_H__
#define __EnemyZombie_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"


struct SDL_Texture;
enum class EnemyZombieDirection
{
	IDLE_R,
	IDLE_L,
	LEFT,
	RIGHT,
	ATTACK_R,
	ATTACK_L,
};

class EnemyZombie : public Entity
{
public:

	EnemyZombie();
	
	virtual ~EnemyZombie();

	bool Awake();

	bool Start();

	bool Update(float dt);


	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);




public:
	float speed = 0.1f;
	//const char* texturePath;
	//SDL_Texture* texture = NULL;

	//PhysBody* pbody;
	//int sniff, fall, jump, blood;
	//bool jumping=false;
	//bool falling = false;
	//bool godmode = false;
	//bool fell = false;
	//float countF = 0.0f;
	bool isAlive = true;
	bool isMovingLeft = false;
	bool isMovingRight = false;
	bool isAttacking = false;
	float cd = 3.0f;
	//bool win = false;

	///*const int NUM_FRAMES = 8;*/
	///*int currentFrame = 0;*/
	//float frameCounter = 0.0f;
	//int frameSpeed = 2;
	//bool collidingPlat;


	SDL_Texture* texture = NULL;
	const char* texturePathEnemyZombie;

	PhysBody* pbody;


	EnemyZombieDirection currentDirection = EnemyZombieDirection::IDLE_L;

	Animation* currentAnimation = &Idle_left;
	Animation Move_left, Move_right, Jump_left, Jump_right, Idle_left, Idle_right, Die_right, Die_left,Attack_right,Attack_left;
	
};

#endif // __EnemyZombie_H__