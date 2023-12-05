#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Module.h"


struct SDL_Texture;
enum class Direction
{
	IDLE_R,
	IDLE_L,
	LEFT,
	RIGHT,
	JUMP_R,
	JUMP_L,
};

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);


	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	//void fireBullet();


public:
	PhysBody* bullet;

	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;

	PhysBody* pbody;
	int sniff, fall, jump, blood;
	bool jumping=false;
	bool falling = false;
	bool godmode = false;
	bool fell = false;
	float countF = 0.0f;
	bool isAlive = true;
	bool win = false;
	int intoxication = 0;

	float frameCounter = 0.0f;
	int frameSpeed = 2;
	bool collidingPlat;

	float cooldown = 0.0f;
	bool isShooting = false;
	bool canShoot = true;

	Direction currentDirection = Direction::IDLE_R;

	Animation* currentAnimation = &Idle_right;
	Animation Move_left, Move_right, Jump_left, Jump_right, Idle_left, Idle_right, Die;
	
};

#endif // __PLAYER_H__