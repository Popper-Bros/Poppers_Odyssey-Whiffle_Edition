#ifndef __Boss_H__
#define __Boss_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Timer.h"

struct SDL_Texture;
enum class BossDirection
{
	IDLE_R,
	IDLE_L,
	LEFT,
	RIGHT,
	ATTACK_R,
	ATTACK_L,
};

class Boss : public Entity
{
public:

	Boss();

	virtual ~Boss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void MoveTowardsNextNode(iPoint& enemyTile, float speed, const DynArray<iPoint>* path, float vely);

	void Attack();

	void Idle();

	bool Die();


public:
	float speed = 1.0f;
	float cooldown = 3.0f;
	bool seePlayer = false;
	int attackSound, deadSound, moveSound;
	bool attackRange = false;
	bool isAttacking = false;

	float timerOn = false;
	Timer timer = Timer();
	float animSpeed = 0.01f;

	iPoint enemyTile;

	iPoint enemyOriginTile;

	const DynArray<iPoint>* path;

	SDL_Texture* texture = NULL;

	BossDirection currentDirection = BossDirection::IDLE_L;

	Animation* currentAnimation = &Idle_left;
	Animation Move_left, Move_right, Idle_left, Idle_right, Die_right, Die_left, Attack_right, Attack_left;

	const char* EnemyHealthBarNullPath;
	const char* EnemyHealthBar1Path;
	const char* EnemyHealthBar2Path;
	const char* EnemyHealthBar3Path;
	const char* EnemyHealthBarFullPath;

	SDL_Texture* EnemyHealthBarNull;
	SDL_Texture* EnemyHealthBar1;
	SDL_Texture* EnemyHealthBar2;
	SDL_Texture* EnemyHealthBar3;
	SDL_Texture* EnemyHealthBarFull;
	SDL_Rect EnemyHealthRec;

};

#endif // __Boss_H__