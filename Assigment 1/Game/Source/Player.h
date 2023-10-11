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
	RIGHT
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

	int Move_Derecha(int &currentF, float &frameC, const int frameS);


public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture_Move_Derecha = NULL;

	PhysBody* pbody;
	int pickCoinFxId;
	bool jumping=false;

	const int NUM_FRAMES = 8;
	int currentFrame = 0;
	float frameCounter = 0.0f;
	const int frameSpeed = 1;

	SDL_Rect destRect;
	SDL_Rect Move_derecha[8];
	SDL_Rect Move_izquierda[8];
	SDL_Rect Idle_right[8];
	SDL_Rect Idle_left[8];

	Direction currentDirection = Direction::IDLE_R;
	SDL_Rect* currentAnimation = Idle_right;
};

#endif // __PLAYER_H__