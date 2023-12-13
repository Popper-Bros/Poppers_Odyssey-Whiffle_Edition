#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "EnemyShadow.h"
#include "EnemyZombie.h"
#include "Particulas.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	int checkpoint;
	
	bool playerAlive = true;

	iPoint getPlayerPos();

	PathFinding* pathfinding;

	iPoint playerTile;

	Particulas* particulas;

	//
	bool LoadState(pugi::xml_node node);

	//
	bool SaveState(pugi::xml_node node);

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	Player* player;
	Item* item;
	EnemyShadow* enemy;
	EnemyZombie* enemy2;
	EnemyShadow* enemy3;
	EnemyZombie* enemy4;
	pugi::xml_node playerConfig;
	pugi::xml_node enemyShadowConfig;
	pugi::xml_node enemyZombieConfig;
	pugi::xml_node enemyShadowConfig2;
	pugi::xml_node enemyZombieConfig2;
	pugi::xml_node particulasConfig;

	SDL_Texture* mouseTileTex = nullptr;

};

#endif // __SCENE_H__