#ifndef __PARTICULAS_H__
#define __PARTICULAS_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Physics.h"
#include <list>

#define MAX_ACTIVE_PARTICLES 3500


struct BalaInfo
{
	//struct donde se guardan los datos de las balas
	PhysBody* balaBody;
	Animation* currentShotAnim;
	Animation shot, endShot;
	bool alive;
	int life = 150;
	int bucle = 0;
	int posy, posx;
	bool collision = false;

	BalaInfo() : balaBody(nullptr), alive (nullptr), currentShotAnim(nullptr), shot(), endShot(), life(0), posx(0), posy(0), collision(nullptr) {}

	BalaInfo(PhysBody* body, Animation _shot, Animation _endShot, int _life, int _bucle, int _posx, int _posy, bool _collision)
		: balaBody(body), shot(_shot), endShot(_endShot), currentShotAnim(&shot), alive(true), life(_life), bucle(_bucle), posx(_posx), posy(_posy), collision(_collision)
	{
		
	}
};

struct SDL_Texture;
enum class tipo
{
	PLAYER_SHOT,
	ENEMY_SHADOW_SHOT,
	UNKNOWN
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

	void Shoot(bool diparar, int positionX, int positionY, int direction, ColliderType coll);


public:

	bool onCollision = false;

	SDL_Texture* texture = NULL;

	bool alive = false;

	Animation* currentShotAnim;
	Animation playerShot, playerShotFinal, enemyShadowShot, enemyShadowShotLeft, enemyShadowShotFinal, enemyShadowShotFinalLeft;

	int balaposx;
	int balaposy;
	int bulletlife = 150;
	int contador = 0;
	float dt;


	List<BalaInfo> balas;

private:

};

#endif // __PARTICULAS_H__