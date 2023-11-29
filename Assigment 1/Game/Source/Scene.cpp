#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Physics.h"
#include "Pathfinding.h"
#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	playerConfig = config.child("player");

	if (config.child("Particulas")) {
		particulas = (Particulas*)app->entityManager->CreateEntity(EntityType::PARTICULAS);
		particulas->parameters = config.child("Particulas");
	}
	particulasConfig = config.child("Particulas");


	if (config.child("EnemyShadow")) {
		enemy = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
		enemy->parameters = config.child("EnemyShadow");
	}

	enemyShadowConfig = config.child("EnemyShadow");

	if (config.child("EnemyZombie")) {
		enemy2 = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
		enemy2->parameters = config.child("EnemyZombie");
	}

	enemyZombieConfig = config.child("EnemyZombie");

	//Get the map name from the config file and assigns the value in the module
	app->map->name = config.child("map").attribute("name").as_string();
	app->map->path = config.child("map").attribute("path").as_string();

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//Initialize pathfinding 
	pathfinding = new PathFinding();

	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music.wav", 1.0f);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;
	checkpoint = 0;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tilewidth,
		app->map->mapData.tileheight,
		app->map->mapData.tilesets.Count());

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (player->isAlive) {
		playerAlive = true;
	}
	else {

		playerAlive = false;
	}

	float camSpeed = 0.125f; 

	//if(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//	app->render->camera.y -= (int)ceil(camSpeed * dt);
	
	//if(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//	app->render->camera.y += (int)ceil(camSpeed * dt);

	if (app->render->camera.x < 0 && ((app->scene->player->position.x) + app->render->camera.x)<(((app->render->camera.w) / 2)) - 40)
		app->render->camera.x += (int)ceil(camSpeed * dt);
	if (app->render->camera.x > -1024 && ((app->scene->player->position.x) + app->render->camera.x) > (((app->render->camera.w) / 2)) + 40)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if (player->position.x > 925 && player->position.y < 200)
	{
		checkpoint = 1;
	}
	if (player->position.x < 150)
	{
		checkpoint = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && player->isAlive) {
		if (player->currentAnimation == &player->Idle_right || player->currentAnimation == &player->Move_right || player->currentAnimation == &player->Jump_right) {
			particulas->Shoot(true, player->position.x, player->position.y - 5, tipo::PLAYER_SHOT,1);
		}
		else if (player->currentAnimation == &player->Idle_left || player->currentAnimation == &player->Move_left || player->currentAnimation == &player->Jump_left) {
			particulas->Shoot(true, player->position.x - 32, player->position.y - 5, tipo::PLAYER_SHOT,-1);
		}
	}

	if (enemy->isAttackingRight && enemy->Attack_right.GetCurrentFrameIndex() == 8 && enemy->cd<=0.0f) {
		enemy->cd = 20.0f;
		particulas->Shoot(true, enemy->position.x, enemy->position.y, tipo::ENEMY_SHADOW_SHOT,1);
	}
	else if (enemy->isAttackingLeft && enemy->Attack_left.GetCurrentFrameIndex() == 8 && enemy->cd <= 0.0f) {
		enemy->cd = 20.0f;
		particulas->Shoot(true, enemy->position.x - 42, enemy->position.y, tipo::ENEMY_SHADOW_SHOT,-1);
	}	

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && player->isAlive && player->fell == false) {
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
		checkpoint == 0;
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && player->isAlive && player->fell == false) {
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(980),PIXEL_TO_METERS(150) }, 0);
		checkpoint == 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && player->isAlive && player->fell == false){
		if (checkpoint==0) player->pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
		else if (checkpoint == 1) player->pbody->body->SetTransform({ PIXEL_TO_METERS(980),PIXEL_TO_METERS(150) }, 0);
	}

	iPoint playerTile = app->map->WorldToMap(player->position.x, player->position.y - app->render->camera.y);

	iPoint enemyTile = app->map->WorldToMap(enemy->position.x, enemy->position.y - app->render->camera.y);


	
	//If mouse button is pressed modify player position
	app->map->pathfinding->CreatePath(enemyTile, playerTile);

	// L13: Get the latest calculated path and draw
	if (app->physics->debug) {
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
		}
	}

	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	//iPoint nextPosition;

	////Check if there is a valid path
	//if (pathfinding->Move(enemyTile, nextPosition))
	//{
	//	MoveObject(currentPosition, nextPosition);
	//	LOG("Moving to (%d, %d)", nextPosition.x, nextPosition.y);
	//}
	//else
	//{
	//	LOG("No valid path available or reached the end of the path.");
	//}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

iPoint Scene::getPlayerPos() {
	return player->position;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
