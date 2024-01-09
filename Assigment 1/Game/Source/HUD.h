#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class HUD : public Module
{
public:

	HUD();

	// Destructor
	virtual ~HUD();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods


public:

	//List<Entity*> entities;
	const char* HealthBar1Path;
	const char* HealthBar2Path;
	const char* HealthBar3Path;
	const char* HealthBar4Path;
	const char* HealthBar5Path;
	const char* HealthBarNullPath;
	const char* BoxPath;
	const char* HeartPath;

	const char* EnemyHealthBarNullPath;
	const char* EnemyHealthBar1Path;
	const char* EnemyHealthBar2Path;
	const char* EnemyHealthBar3Path;
	const char* EnemyHealthBarFullPath;

	const char* popperEmptyPath;
	const char* popperFullPath;

	SDL_Texture* HealthBarNull;
	SDL_Texture* HealthBar1;
	SDL_Texture* HealthBar2;
	SDL_Texture* HealthBar3;
	SDL_Texture* HealthBar4;
	SDL_Texture* HealthBar5;
	SDL_Texture* Box;
	SDL_Texture* Heart;
	SDL_Rect box, HealthRec, heart;

	SDL_Texture* EnemyHealthBarNull;
	SDL_Texture* EnemyHealthBar1;
	SDL_Texture* EnemyHealthBar2;
	SDL_Texture* EnemyHealthBar3;
	SDL_Texture* EnemyHealthBarFull;
	SDL_Rect EnemyHealthRec;

	SDL_Texture* popperEmpty;
	SDL_Texture* popperFull;
	SDL_Rect popperRec;
	SDL_Rect popperRec2;
	SDL_Rect popperRec3;
	

	int playerHealth = 1;

private:

};

#endif // __HUD_H__
