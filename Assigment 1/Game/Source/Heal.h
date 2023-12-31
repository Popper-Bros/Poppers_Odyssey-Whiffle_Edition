#ifndef __HEAL_H__
#define __HEAL_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum class HealState
{
	IDLE,
	PICKED
};
class Heal : public Entity
{
public:

	Heal();
	virtual ~Heal();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	PhysBody* pbody;
	HealState state = HealState::IDLE;
	Animation* currentAnimation = &Idle;
	Animation Idle, Pick;

private:

	SDL_Texture* texture;
};

#endif // __HEAL_H__