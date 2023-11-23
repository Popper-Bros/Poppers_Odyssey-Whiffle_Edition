#ifndef __PARTICULAS_H__
#define __PARTICULAS_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include <list>

#define MAX_ACTIVE_PARTICLES 3500


struct BalaInfo
{
	PhysBody* balaBody;
	int life;

	BalaInfo(PhysBody* body, int l) : balaBody(body), life(l) {}
};

struct SDL_Texture;
enum class shooting
{
	ALIVE,
	DEAD
};

class Particulas : public Entity
{
public:

	Particulas();
	virtual ~Particulas();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Shoot(bool diparar, int positionX, int positionY);

	void unShot();

public:

	bool onCollision = false;

	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Rect rec;

	PhysBody* bala;

	bool disparar = false;
	Animation* currentShotAnim = &shot;
	Animation shot, endShot;
	int balaposx;
	int balaposy;
	int bulletlife = 150;
	int contador = 0;

	shooting currentState;

	List<BalaInfo> balas;

private:

};

#endif // __PARTICULAS_H__