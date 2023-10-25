#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
enum class state
{
	PICKED_TRUE,
	PICKED_FALSE
};

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;
	state Picked = state::PICKED_FALSE;
	PhysBody* pbody;

private:

	SDL_Texture* texture;
	const char* texturePath;
};

#endif // __ITEM_H__