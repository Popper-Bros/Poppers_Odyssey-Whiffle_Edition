#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"


struct SDL_Texture;
enum class EnemyDirection
{
	IDLE_R,
	IDLE_L,
	LEFT,
	RIGHT,
	JUMP_R,
	JUMP_L,
};

class Enemy : public Entity
{
public:

	Enemy();
	
	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update(float dt);


	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);




public:
	//float speed = 0.2f;
	//const char* texturePath;
	//SDL_Texture* texture = NULL;

	//PhysBody* pbody;
	//int sniff, fall, jump, blood;
	//bool jumping=false;
	//bool falling = false;
	//bool godmode = false;
	//bool fell = false;
	//float countF = 0.0f;
	//bool isAlive = true;
	//bool win = false;

	///*const int NUM_FRAMES = 8;*/
	///*int currentFrame = 0;*/
	//float frameCounter = 0.0f;
	//int frameSpeed = 2;
	//bool collidingPlat;


	SDL_Texture* texture = NULL;
	const char* texturePathEnemy;

	PhysBody* pbody;


	EnemyDirection currentDirection = EnemyDirection::IDLE_L;

	Animation* currentAnimation = &Idle_left;
	Animation Move_left, Move_right, Jump_left, Jump_right, Idle_left, Idle_right, Die;
	
};

#endif // __ENEMY_H__