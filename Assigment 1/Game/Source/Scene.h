#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Heal.h"
#include "EnemyShadow.h"
#include "EnemyZombie.h"
#include "Boss.h"
#include "Particulas.h"
#include "HUD.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "Animation.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	void AwakeForLevel1(const pugi::xml_node& conf);

	void AwakeForLevel2(const pugi::xml_node& conf);

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

	int playerItem = 0;

	bool tp1 = false;

	bool tp2 = false;

	iPoint getPlayerPos();

	int GetPlayerLife();

	PathFinding* pathfinding;

	iPoint playerTile;

	Particulas* particulas;

	//
	bool LoadState(pugi::xml_node node);

	//
	bool SaveState(pugi::xml_node node);

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool isPaused = false;

	Player* player;

	Boss* boss;

	pugi::xml_node savedConfig;

	GuiControlSlider* Slider;
	GuiControlButton* gcButton;

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	
	Item* item;
	Heal* heal;
	EnemyShadow* enemyShadow;
	EnemyZombie* enemyZombie;

	SDL_Texture* mouseTileTex = nullptr;

	//GuiControlButton* gcButton;
	int variable = 0;
};

#endif // __SCENE_H__