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

	enemyShadowConfig2 = config.child("EnemyShadow");

	if (config.child("EnemyShadow2")) {
		enemy3 = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
		enemy3->parameters = config.child("EnemyShadow2");
	}

	enemyShadowConfig2 = config.child("EnemyShadow2");

	if (config.child("EnemyZombie")) {
		enemy2 = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
		enemy2->parameters = config.child("EnemyZombie");
	}

	enemyZombieConfig = config.child("EnemyZombie");

	if (config.child("EnemyZombie2")) {
		enemy4 = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
		enemy4->parameters = config.child("EnemyZombie2");
	}

	enemyZombieConfig2 = config.child("EnemyZombie2");

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

	playerTile = app->map->WorldToMap(25+player->position.x, player->position.y - app->render->camera.y);
		
	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();


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

bool Scene::LoadState(pugi::xml_node node) {

	//for (pugi::xml_node itemNode = node.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	//{
	//	item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	//	item->parameters = itemNode;
	//}

	if (node.child("Player")) {
		player->position.x = node.child("Player").attribute("x").as_int();
		player->position.y = node.child("Player").attribute("y").as_int();
		player->intoxication = node.child("intoxication").attribute("x").as_int();
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(player->position.x+0.3f),PIXEL_TO_METERS(player->position.y+0.3f) }, 0);
	}

	if (node.child("EnemyShadow")) {
		if (node.child("EnemyShadow").attribute("isAlive").as_bool() == true && enemy->isAlive == false) {
			enemy = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
			enemy->isAlive = true;
			enemy->Awake();
			enemy->Start();
		}
		enemy->position.x = node.child("EnemyShadow").attribute("x").as_int();
		enemy->position.y = node.child("EnemyShadow").attribute("y").as_int();

		enemy->pbody->body->SetTransform({ PIXEL_TO_METERS(enemy->position.x+0.4f),PIXEL_TO_METERS(enemy->position.y+0.73f) }, 0);

	}

	if (node.child("EnemyZombie")) {
		if (node.child("EnemyZombie").attribute("isAlive").as_bool() == true && enemy2->isAlive == false) {
			enemy2 = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
			enemy2->isAlive = true;
			enemy2->Awake();
			enemy2->Start();
		}
		enemy2->position.x = node.child("EnemyZombie").attribute("x").as_int();
		enemy2->position.y = node.child("EnemyZombie").attribute("y").as_int();

		enemy2->pbody->body->SetTransform({ PIXEL_TO_METERS(enemy2->position.x+0.6f),PIXEL_TO_METERS(enemy2->position.y+0.93f) }, 0);
	}

	if (node.child("EnemyShadow2")) {
		if (node.child("EnemyShadow2").attribute("isAlive").as_bool() == true && enemy3->isAlive == false) {
			enemy3 = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
			enemy3->isAlive = true;
			enemy3->Awake();
			enemy3->Start();
		}
		enemy3->position.x = node.child("EnemyShadow2").attribute("x").as_int();
		enemy3->position.y = node.child("EnemyShadow2").attribute("y").as_int();

		enemy3->pbody->body->SetTransform({ PIXEL_TO_METERS(enemy3->position.x + 0.4f),PIXEL_TO_METERS(enemy3->position.y + 0.73f) }, 0);

	}

	if (node.child("EnemyZombie2")) {
		if (node.child("EnemyZombie2").attribute("isAlive").as_bool() == true && enemy4->isAlive == false) {
			enemy4 = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
			enemy4->isAlive = true;
			enemy4->Awake();
			enemy4->Start();
		}
		enemy4->position.x = node.child("EnemyZombie2").attribute("x").as_int();
		enemy4->position.y = node.child("EnemyZombie2").attribute("y").as_int();

		enemy4->pbody->body->SetTransform({ PIXEL_TO_METERS(enemy4->position.x + 0.6f),PIXEL_TO_METERS(enemy4->position.y + 0.93f) }, 0);
	}

	for (pugi::xml_node itemNode = node.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		if (node.child("item").attribute("isAlive").as_bool() == true && item->isAlive == false) {
			item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM); 
			item->isAlive = true;
			item->Awake();
			item->Start();
		}
		item->position.x = node.child("item").attribute("x").as_int();
		item->position.y = node.child("item").attribute("y").as_int();

		item->pbody->body->SetTransform({ PIXEL_TO_METERS(item->position.x),PIXEL_TO_METERS(item->position.y) }, 0);
	}

	return true;
}

bool Scene::SaveState(pugi::xml_node node) {
	//pugi::xml_node entNode = node.append_child("entitymanager");
	for (int i = 0; i < app->entityManager->entities.Count(); i++)
	{
		pugi::xml_node entNode = node.append_child(app->entityManager->entities[i]->name.GetString());
		entNode.append_attribute("x").set_value(app->entityManager->entities[i]->position.x);
		entNode.append_attribute("y").set_value(app->entityManager->entities[i]->position.y);
		entNode.append_attribute("isAlive").set_value(app->entityManager->entities[i]->isAlive);
		if (app->entityManager->entities[i]->name == "Player") {
			entNode.append_attribute("intoxication").set_value(app->entityManager->entities[i]->intoxication);
		}
	}

	return true;
}