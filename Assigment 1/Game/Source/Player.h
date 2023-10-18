#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
enum class Direction
{
	IDLE_R,
	IDLE_L,
	LEFT,
	RIGHT,
	JUMP_R,
	JUMP_L,
	DIE
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


public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;

	PhysBody* pbody;
	int sniff;
	bool jumping=false;
	bool falling = false;
	bool godmode = false;
	float countF = 0.0f;

	const int NUM_FRAMES = 8;
	int currentFrame = 0;
	float frameCounter = 0.0f;
	const int frameSpeed = 2;
	bool collidingPlat;

	SDL_Rect destRect;
	SDL_Rect Move_derecha[8];
	SDL_Rect Move_izquierda[8];
	SDL_Rect Idle_right[8];
	SDL_Rect Idle_left[8];
	SDL_Rect Jump_right[8];
	SDL_Rect Jump_left[8];
	SDL_Rect Die[8];

	Direction currentDirection = Direction::IDLE_R;
	SDL_Rect* currentAnimation = Idle_right;
};

#endif // __PLAYER_H__