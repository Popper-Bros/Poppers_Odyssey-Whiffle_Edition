#include "HUD.h"
#include "EntityManager.h"
#include "Player.h"
#include "Item.h"
#include "Heal.h"
#include "EnemyShadow.h"
#include "EnemyZombie.h"
#include "Particulas.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Defs.h"
#include "Log.h"
#include "Player.h"

HUD::HUD() : Module()
{
	name.Create("HUD");
}

// Destructor
HUD::~HUD()
{}

// Called before render is available
bool HUD::Awake(pugi::xml_node& config)
{
    HealthBar1Path = config.child("HealthBar1").attribute("texturepath").as_string();
    HealthBar2Path = config.child("HealthBar2").attribute("texturepath").as_string();
    HealthBar3Path = config.child("HealthBar3").attribute("texturepath").as_string();
    HealthBar4Path = config.child("HealthBar4").attribute("texturepath").as_string();
    HealthBar5Path = config.child("HealthBar5").attribute("texturepath").as_string();
    HealthBarNullPath = config.child("HealthBarNull").attribute("texturepath").as_string();
    BoxPath = config.child("Box").attribute("texturepath").as_string();
    HeartPath = config.child("Heart").attribute("texturepath").as_string();

	EnemyHealthBarNullPath = config.child("EnemyHealthNull").attribute("texturepath").as_string();
	EnemyHealthBar1Path = config.child("EnemyHealth1").attribute("texturepath").as_string();
	EnemyHealthBar2Path = config.child("EnemyHealth2").attribute("texturepath").as_string();
	EnemyHealthBar3Path = config.child("EnemyHealth3").attribute("texturepath").as_string();
	EnemyHealthBarFullPath = config.child("EnemyHealthFull").attribute("texturepath").as_string();

	popperEmptyPath = config.child("PopperEmpty").attribute("texturepath").as_string();
	popperFullPath = config.child("PopperFull").attribute("texturepath").as_string();

	numbersPath = config.child("Numbers").attribute("texturepath").as_string();

	return true;
}

bool HUD::Start() {
    HealthBar1 = app->tex->Load(HealthBar1Path);
    HealthBar2 = app->tex->Load(HealthBar2Path);
    HealthBar3 = app->tex->Load(HealthBar3Path);
    HealthBar4 = app->tex->Load(HealthBar4Path);
    HealthBar5 = app->tex->Load(HealthBar5Path);
    HealthBarNull = app->tex->Load(HealthBarNullPath);
    Box = app->tex->Load(BoxPath);
    Heart = app->tex->Load(HeartPath);

	EnemyHealthBarNull = app->tex->Load(EnemyHealthBarNullPath);
	EnemyHealthBar1 = app->tex->Load(EnemyHealthBar1Path);
	EnemyHealthBar2 = app->tex->Load(EnemyHealthBar2Path);
	EnemyHealthBar3 = app->tex->Load(EnemyHealthBar3Path);
	EnemyHealthBarFull = app->tex->Load(EnemyHealthBarFullPath);

	popperEmpty = app->tex->Load(popperEmptyPath);
	popperFull = app->tex->Load(popperFullPath);
	
	numbers = app->tex->Load(numbersPath);
	

    HealthRec = { 76, 18, 28, 12 };
    box = { 68, 14, 43, 20 };
    heart = { 64, 19, 10, 10 };
	popperRec = { 66, 40, 16, 20 };
	popperRec2 = { 81, 40, 16, 20 };
	popperRec3 = { 96, 40, 16, 20 };
	for (int i = 0; i < 10; i++)
	{
		Num[i] = { 11*(i+1), 0, 9, 20 };
	}

	timerRect = { 500, 20, 9, 20 };

	SDL_SetTextureAlphaMod(popperEmpty, 100);

	return true;
}

bool HUD::Update(float dt)
{
	if (!timerOn)
	{
		timer.Start();
		timerOn = true;
		elapsedTime = 0;
	}
	elapsedTime = timer.ReadSec();
	LOG("%d", timer.ReadSec());
	if (elapsedTime > 10)
	{
		timerOn = false;
	}
	switch (elapsedTime)
	{
	case 0:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[0], &timerRect);
		break;
	case 1:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[1], &timerRect);
		break;
	case 2:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[2], &timerRect);
		break;
	case 3:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[3], &timerRect);
		break;
	case 4:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[4], &timerRect);
		break;
	case 5:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[5], &timerRect);
		break;
	case 6:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[6], &timerRect);
		break;
	case 7:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[7], &timerRect);
		break;
	case 8:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[8], &timerRect);
		break;
	case 9:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[9], &timerRect);
		break;
	case 10:
		SDL_RenderCopy(app->render->renderer, numbers, &Num[0], &timerRect);
		break;
	}
	
	playerHealth = app->scene->GetPlayerLife();

	SDL_RenderCopy(app->render->renderer, Box, NULL, &box);
	SDL_RenderCopy(app->render->renderer, Heart, NULL, &heart);

		

	switch (app->scene->playerItem)
	{
	case 0:
		SDL_RenderCopy(app->render->renderer, popperEmpty, NULL, &popperRec);
		SDL_RenderCopy(app->render->renderer, popperEmpty, NULL, &popperRec2);
		SDL_RenderCopy(app->render->renderer, popperEmpty, NULL, &popperRec3);
		break;
	case 1:
		SDL_RenderCopy(app->render->renderer, popperFull, NULL, &popperRec);
		SDL_RenderCopy(app->render->renderer, popperEmpty, NULL, &popperRec2);
		SDL_RenderCopy(app->render->renderer, popperEmpty, NULL, &popperRec3);
		break;
	case 2:
		SDL_RenderCopy(app->render->renderer, popperFull, NULL, &popperRec);
		SDL_RenderCopy(app->render->renderer, popperFull, NULL, &popperRec2);
		SDL_RenderCopy(app->render->renderer, popperEmpty, NULL, &popperRec3);
		break;
	case 3:
		SDL_RenderCopy(app->render->renderer, popperFull, NULL, &popperRec);
		SDL_RenderCopy(app->render->renderer, popperFull, NULL, &popperRec2);
		SDL_RenderCopy(app->render->renderer, popperFull, NULL, &popperRec3);
		break;
	default:
		break;
	}

	switch (playerHealth)
	{
	case 5:
		SDL_RenderCopy(app->render->renderer, HealthBar5, NULL, &HealthRec);
		break;
	case 4:
		SDL_RenderCopy(app->render->renderer, HealthBar4, NULL, &HealthRec);
		break;
	case 3:
		SDL_RenderCopy(app->render->renderer, HealthBar3, NULL, &HealthRec);
		break;
	case 2:
		SDL_RenderCopy(app->render->renderer, HealthBar2, NULL, &HealthRec);
		break;
	case 1:
		SDL_RenderCopy(app->render->renderer, HealthBar1, NULL, &HealthRec);
		break;
	case 0:
		SDL_RenderCopy(app->render->renderer, HealthBarNull, NULL, &HealthRec);
		break;
	}

	
	return true;
}

// Called before quitting
bool HUD::CleanUp()
{
	app->tex->UnLoad(HealthBar1);
	app->tex->UnLoad(HealthBar2);
	app->tex->UnLoad(HealthBar3);
	app->tex->UnLoad(HealthBar4);
	app->tex->UnLoad(HealthBar5);
	app->tex->UnLoad(HealthBarNull);
	app->tex->UnLoad(Box);
	app->tex->UnLoad(Heart);
	app->tex->UnLoad(popperEmpty);
	app->tex->UnLoad(popperFull);
	return true;
}

