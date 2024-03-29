#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "DynArray.h"


class PhysBody;
class App;

enum class EntityType
{
	PLAYER,
	ITEM,
	HEAL,
	ENEMYSHADOW,
	ENEMYZOMBIE,
	PARTICULAS,
	BOSS,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
			
		}
	}

	void MoveTowardsNextNode(iPoint& enemyTile, float speed, const DynArray<iPoint>* path) {

	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};



public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;
	PhysBody* pbody;
	bool renderable = true;
	bool isAlive = true;
	int intoxication;
	int health;
	const char* texturepath;
	int Num;
	float animSpeed = 0.01f;

	SDL_Texture* mouseTileTex = nullptr;
};

#endif // __ENTITY_H__