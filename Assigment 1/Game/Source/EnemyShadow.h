#ifndef __EnemyShadow_H__
#define __EnemyShadow_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "DynArray.h"
#include "SDL/include/SDL.h"
#include "Timer.h"


struct SDL_Texture;
enum class EnemyShadowDirection
{
	LEFT,
	RIGHT,
	ATTACK_R,
	ATTACK_L,
};

class EnemyShadow : public Entity
{
public:

	EnemyShadow();
	
	virtual ~EnemyShadow();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void MoveTowardsNextNode(iPoint& enemyTile, float speed, const DynArray<iPoint>* path);

	void Attack();

	bool Die();

public:
	float speed = 1.0f;
	//const char* texturePath;
	//SDL_Texture* texture = NULL;

	//PhysBody* pbody;
	//int sniff, fall, jump, blood;
	//bool jumping=false;
	//bool falling = false;
	//bool godmode = false;
	//bool fell = false;
	//float countF = 0.0f;
	bool isMovingLeft = false;
	bool isMovingRight = false;
	bool isAttackingLeft = false;
	bool isAttackingRight = false;
	bool seePlayer = false;
	float cd = 3.0f;
	//bool win = false;
	int shot;
	int death;
	///*const int NUM_FRAMES = 8;*/
	///*int currentFrame = 0;*/
	//float frameCounter = 0.0f;
	//int frameSpeed = 2;
	//bool collidingPlat;

	float timerOn = false;
	Timer timer = Timer();

	iPoint enemyTile;

	iPoint enemyOriginTile;

	const DynArray<iPoint>* path;

	SDL_Texture* texture = NULL;

	EnemyShadowDirection currentDirection = EnemyShadowDirection::LEFT;

	Animation* currentAnimation = &Idle_left;
	Animation Idle_left, Idle_right, Die_right, Die_left,Attack_right,Attack_left;

	const char* EnemyHealthBarNullPath;
	const char* EnemyHealthBar1Path;
	const char* EnemyHealthBar2Path;
	const char* EnemyHealthBarFullPath;

	SDL_Texture* EnemyHealthBarNull;
	SDL_Texture* EnemyHealthBar1;
	SDL_Texture* EnemyHealthBar2;
	SDL_Texture* EnemyHealthBarFull;
	SDL_Rect EnemyHealthRec;
	
};

#endif // __EnemyShadow_H__