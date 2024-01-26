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
#include "HUD.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Optick/include/optick.h"

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

	if (config.child("Particulas")) {
		particulas = (Particulas*)app->entityManager->CreateEntity(EntityType::PARTICULAS);
		particulas->parameters = config.child("Particulas");
	}

	if (app->map->level == 1) {
		// iterate all objects in the scene
		// Check https://pugixml.org/docs/quickstart.html#access

		AwakeForLevel1(config);

	}
	
	else if (app->map->level == 2) {

		AwakeForLevel2(config);
	}

	//Get the map name from the config file and assigns the value in the module
	app->map->name1 = config.child("map").attribute("name1").as_string();
	app->map->name2 = config.child("map").attribute("name2").as_string();
	app->map->path = config.child("map").attribute("path").as_string();

	savedConfig = config;

	return ret;
}

void Scene::AwakeForLevel1(const pugi::xml_node& config)
{
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item")) {
		if (itemNode.attribute("scene").as_int() == 1) {
			item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
			this->item->parameters = itemNode;
		}
	}

	for (pugi::xml_node playerNode = config.child("player"); playerNode; playerNode = playerNode.next_sibling("player")) {
		if (playerNode.attribute("scene").as_int() == 1) {
			player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
			this->player->parameters = playerNode;
		}
	}

	for (pugi::xml_node bossNode = config.child("Boss"); bossNode; bossNode = bossNode.next_sibling("Boss")) {
		if (bossNode.attribute("scene").as_int() == 1) {
			boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
			this->boss->parameters = bossNode;
		}
	}

	for (pugi::xml_node enemyShadowNode = config.child("EnemyShadow"); enemyShadowNode; enemyShadowNode = enemyShadowNode.next_sibling("EnemyShadow")) {
		if (enemyShadowNode.attribute("scene").as_int() == 1) {
			enemyShadow = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
			this->enemyShadow->parameters = enemyShadowNode;
		}
	}

	for (pugi::xml_node enemyZombieNode = config.child("EnemyZombie"); enemyZombieNode; enemyZombieNode = enemyZombieNode.next_sibling("EnemyZombie")) {
		if (enemyZombieNode.attribute("scene").as_int() == 1) {
			enemyZombie = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
			this->enemyZombie->parameters = enemyZombieNode;
		}
	}

	for (pugi::xml_node healNode = config.child("Heal"); healNode; healNode = healNode.next_sibling("Heal")) {
		if (healNode.attribute("scene").as_int() == 1) {
			heal = (Heal*)app->entityManager->CreateEntity(EntityType::HEAL);
			this->heal->parameters = healNode;
		}
	}
}

void Scene::AwakeForLevel2(const pugi::xml_node& config) {
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item")) {
		if (itemNode.attribute("scene").as_int() == 2) {
			item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
			this->item->parameters = itemNode;
		}
	}

	for (pugi::xml_node playerNode = config.child("player"); playerNode; playerNode = playerNode.next_sibling("player")) {
		if (playerNode.attribute("scene").as_int() == 2) {
			player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
			this->player->parameters = playerNode;
		}
	}

	for (pugi::xml_node bossNode = config.child("Boss"); bossNode; bossNode = bossNode.next_sibling("Boss")) {
		if (bossNode.attribute("scene").as_int() == 2) {
			boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
			this->boss->parameters = bossNode;
		}
	}

	for (pugi::xml_node enemyShadowNode = config.child("EnemyShadow"); enemyShadowNode; enemyShadowNode = enemyShadowNode.next_sibling("EnemyShadow")) {
		if (enemyShadowNode.attribute("scene").as_int() == 2) {
			enemyShadow = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
			this->enemyShadow->parameters = enemyShadowNode;
		}
	}

	for (pugi::xml_node enemyZombieNode = config.child("EnemyZombie"); enemyZombieNode; enemyZombieNode = enemyZombieNode.next_sibling("EnemyZombie")) {
		if (enemyZombieNode.attribute("scene").as_int() == 2) {
			enemyZombie = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
			this->enemyZombie->parameters = enemyZombieNode;
		}
	}

	for (pugi::xml_node healNode = config.child("Heal"); healNode; healNode = healNode.next_sibling("Heal")) {
		if (healNode.attribute("scene").as_int() == 2) {
			heal = (Heal*)app->entityManager->CreateEntity(EntityType::HEAL);
			this->heal->parameters = healNode;
		}
	}
}

// Called before the first frame
bool Scene::Start()
{
	//Initialize pathfinding 
	pathfinding = new PathFinding();

	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	

	//Music is commented so that you can add your own music
	app->audio->PlayMusic("Assets/Audio/Music/music.ogg");

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

	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;
	app->SaveRequest();

	SDL_Rect btPos = { windowW / 2 - 60,20, 120,12 };
	//gcButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Didac, el virgen", btPos, this);
	Slider = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "", btPos, this);

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


	app->physics->step = 1.0f / 60.0f;
	switch (player->itemPicked)
	{
	case 0:
		playerItem = 0;
		break;
	case 1:
		playerItem = 1;
		break;
	case 2:
		playerItem = 2;
		break;
	case 3:
		playerItem = 3;
		break;

	}

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

	//camera movement
	if (app->render->camera.x < 0 && ((app->scene->player->position.x) + app->render->camera.x) < (((app->render->camera.w) / 2)) - 40)
		app->render->camera.x += (int)ceil(camSpeed * dt);
	if (app->render->camera.x > -1024 && ((app->scene->player->position.x) + app->render->camera.x) > (((app->render->camera.w) / 2)) + 40)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if (player->position.x > 925 && player->position.y < 200 && checkpoint == 0 && app->map->level == 1)
	{
		app->SaveRequest();
		checkpoint = 1;
	}

	if (player->position.x > 810 && player->position.y < 200 && checkpoint == 0 && app->map->level == 2)
	{
		app->SaveRequest();
		checkpoint = 1;
	}

	//el player vuelve al pricipio 
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && player->isAlive && player->fell == false) {
		checkpoint = 0;
		app->render->camera.x = 0;
		if (app->map->level == 1) {
			player->pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
		}
		else if (app->map->level == 2) {
			player->pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(530) }, 0);
		}
		app->SaveRequest();
	}
	//el player vuelve al checkpoint
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && player->isAlive && player->fell == false) {
		checkpoint = 1;
		app->render->camera.x = -435;
		if (app->map->level == 1) {
			player->pbody->body->SetTransform({ PIXEL_TO_METERS(980),PIXEL_TO_METERS(150) }, 0);
		}
		else if (app->map->level == 2) {
			player->pbody->body->SetTransform({ PIXEL_TO_METERS(810),PIXEL_TO_METERS(60) }, 0);
		}
		app->SaveRequest();
	}
	//el player respawnea en el ultimo checkpoint
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && player->isAlive && player->fell == false) {
		if (checkpoint == 0) player->pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
		else if (checkpoint == 1) player->pbody->body->SetTransform({ PIXEL_TO_METERS(980),PIXEL_TO_METERS(150) }, 0);
	}

	playerTile = app->map->WorldToMap(25 + player->position.x, player->position.y - app->render->camera.y);

	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)

	//hace un save
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	//hace un load
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	//teleport 1 level 2
	if (player->position.x > 1020 && player->position.x < 1060 && player->position.y > 650) {
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(1294),PIXEL_TO_METERS(94) }, 0);
		tp1 = true;
	}

	//teleport 2 level 2
	if (player->position.x > 1275 && player->position.x < 1310 && player->position.y > 650) {
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(1423),PIXEL_TO_METERS(94) }, 0);
		tp2 = true;
	}

	if (player->win && app->map->level == 1 || app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		CleanUp();
		app->map->CleanUp();
		app->physics->CleanUp();
		app->hud->CleanUp();
		app->tex->CleanUp();
		app->map->level = 2;
		player->win = false;
		//player->win = false;
		//player->pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
		app->tex->Awake(savedConfig);
		app->map->Awake(savedConfig);
		Awake(savedConfig);
		player->Awake();
		app->entityManager->Awake(savedConfig);
		app->hud->Awake(savedConfig);
		app->tex->Start();
		app->physics->Start();
		app->map->Start();
		Start();
		app->entityManager->Start();
		app->hud->Start();
		app->render->camera.x = 0;
	}
	if (isPaused)
	{
		app->win->GetWindowSize(windowW, windowH);
		SDL_Rect pos = { windowW/2-40, windowH/2-100, 80,30 };
		SDL_Rect pos2 = { windowW / 2 - 40, windowH / 2 - 50, 80,30 };
		SDL_Rect pos3 = { windowW / 2 - 40, windowH / 2, 80,30 };
		SDL_Rect pos4 = { windowW / 2 - 40, windowH / 2 + 50, 80,30 };
		SDL_Rect pos5 = { windowW / 2 - 40, windowH / 2 + 100, 80,30 };

		if (variable == 0)
		{
			gcButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "RESUME", pos, this);
			gcButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "SETTINGS", pos2, this);
			gcButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "BACK to TITLE", pos3, this);
			gcButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "EXIT", pos4, this);
			
			variable++;
		}
		
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(esc || app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		ret = false;
	//make a pause menu
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		isPaused = true;
		
	}
	if (isPaused)
	{
		app->guiManager->active = true;
		//pause the game
		app->physics->step = 0;
		app->audio->active = false;
		for (int i = 0; i < app->entityManager->entities.Count(); i++)
		{
			app->entityManager->entities[i]->animSpeed = 0;
		}
		
	}
	else if (!isPaused)
	{
		variable = 0;
		app->physics->step = 1.0f / 60.0f;
		app->audio->active = true;
		for (int i = 0; i < app->entityManager->entities.Count(); i++)
		{
			app->entityManager->entities[i]->animSpeed = 0.01;
		}
		//unload the button
		if (gcButton != nullptr)
		{
			app->guiManager->active = false;
		}

	}

	return ret;
}

iPoint Scene::getPlayerPos() {
	return player->position;
}	

int Scene::GetPlayerLife() {
	return player->health;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->entityManager->CleanUp();

	return true;
}

bool Scene::LoadState(pugi::xml_node node) // esta funcion carga los datos del xml en las variables de los objetos
{

	//for (pugi::xml_node itemNode = node.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	//{
	//	item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	//	item->parameters = itemNode;
	//}
	
	for (pugi::xml_node playerNode = node.child("Player"); playerNode; playerNode = playerNode.next_sibling("Player")) {
		this->player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		this->player->parameters = playerNode;
		this->player->Awake();
		this->player->texturepath = playerNode.attribute("texturepath").as_string();
		this->player->Start();
		this->player->isAlive = true;
		this->player->position.x = node.child("Player").attribute("x").as_int();
		this->player->position.y = node.child("Player").attribute("y").as_int();
		this->player->intoxication = node.child("Player").attribute("intoxication").as_int();
		this->player->health = node.child("Player").attribute("health").as_int();
		this->player->itemPicked = node.child("Player").attribute("itemPicked").as_int();
		

	}

	for (pugi::xml_node healNode = node.child("Heal"); healNode; healNode = healNode.next_sibling("Heal")) {
		if (healNode.attribute("isAlive").as_bool() == true && heal->isAlive == false) {
			this->heal = (Heal*)app->entityManager->CreateEntity(EntityType::HEAL);
			this->heal->parameters = healNode;
			this->heal->Awake();
			this->heal->texturepath = healNode.attribute("texturepath").as_string();
			this->heal->Start();
			this->heal->isAlive = true;
		}
		if (healNode.attribute("isAlive").as_bool() == false && heal->isAlive == true) {
			this->heal->isAlive = false;
		}
		this->heal->position.x = node.child("Heal").attribute("x").as_int();
		this->heal->position.y = node.child("Heal").attribute("y").as_int();
		this->heal->pbody->body->SetTransform({ PIXEL_TO_METERS(heal->position.x) + PIXEL_TO_METERS(9),PIXEL_TO_METERS(heal->position.y) + PIXEL_TO_METERS(8) }, 0);
	}

	for (pugi::xml_node enemyShadowNode = node.child("EnemyShadow"); enemyShadowNode; enemyShadowNode = enemyShadowNode.next_sibling("EnemyShadow")) {
		if (enemyShadowNode.attribute("isAlive").as_bool() == true && this->enemyShadow->isAlive == false) {
			this->enemyShadow = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
			this->enemyShadow->parameters = enemyShadowNode;
			this->enemyShadow->Awake();
			this->enemyShadow->texturepath = enemyShadowNode.attribute("texturepath").as_string();
			this->enemyShadow->Start();
			this->enemyShadow->isAlive = true;
		}
		this->enemyShadow->position.x = enemyShadowNode.attribute("x").as_int();
		this->enemyShadow->position.y = enemyShadowNode.attribute("y").as_int();

		this->enemyShadow->pbody->body->SetTransform({ PIXEL_TO_METERS(enemyShadow->position.x + 0.4f),PIXEL_TO_METERS(enemyShadow->position.y + 0.73f) }, 0);
	}

	for (pugi::xml_node enemyZombieNode = node.child("EnemyZombie"); enemyZombieNode; enemyZombieNode = enemyZombieNode.next_sibling("EnemyZombie")) {
		if (enemyZombieNode.attribute("isAlive").as_bool() == true && this->enemyZombie->isAlive == false) {
			this->enemyZombie = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
			this->enemyZombie->parameters = enemyZombieNode;
			this->enemyZombie->Awake();
			this->enemyZombie->texturepath = enemyZombieNode.attribute("texturepath").as_string();
			this->enemyZombie->Start();
			this->enemyZombie->isAlive = true;
		}
		this->enemyZombie->position.x = enemyZombieNode.attribute("x").as_int();
		this->enemyZombie->position.y = enemyZombieNode.attribute("y").as_int();

		this->enemyZombie->pbody->body->SetTransform({ PIXEL_TO_METERS(enemyZombie->position.x + 0.6f),PIXEL_TO_METERS(enemyZombie->position.y + 0.93f) }, 0);
	}

	for (pugi::xml_node itemNode = node.child("item"); itemNode; itemNode = itemNode.next_sibling("item")) {
			if (itemNode.attribute("isAlive").as_bool() == true && item->isAlive == false) {
				this->item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
				this->item->parameters = itemNode;
				this->item->Awake();
				this->item->texturepath = itemNode.attribute("texturepath").as_string();
				this->item->Start();
				this->item->isAlive = true;
			}
			if (itemNode.attribute("isAlive").as_bool() == false && item->isAlive == true) {
				this->item->isAlive = false;
			}
			this->item->position.x = itemNode.attribute("x").as_int();
			this->item->position.y = itemNode.attribute("y").as_int();

			this->item->pbody->body->SetTransform({ PIXEL_TO_METERS(item->position.x) + PIXEL_TO_METERS(9),PIXEL_TO_METERS(item->position.y) + PIXEL_TO_METERS(8) }, 0);
	}

	return true;
}

bool Scene::SaveState(pugi::xml_node node) // esta funcion guarda los datos de las variables de los objetos en el xml "save_game" 
{
	//pugi::xml_node entNode = node.append_child("entitymanager");
	for (int i = 0; i < app->entityManager->entities.Count(); i++)
	{
		pugi::xml_node entNode = node.append_child(app->entityManager->entities[i]->name.GetString());
		entNode.append_attribute("x").set_value(app->entityManager->entities[i]->position.x);
		entNode.append_attribute("y").set_value(app->entityManager->entities[i]->position.y);
		entNode.append_attribute("isAlive").set_value(app->entityManager->entities[i]->isAlive);
		entNode.append_attribute("texturepath").set_value(app->entityManager->entities[i]->texturepath);
		if (app->entityManager->entities[i]->name == "Player") {
			entNode.append_attribute("intoxication").set_value(app->entityManager->entities[i]->intoxication);
			entNode.append_attribute("health").set_value(app->entityManager->entities[i]->health);
			entNode.append_attribute("itemPicked").set_value(player->itemPicked);
		}
		/*if (app->entityManager->entities[i]->name == "EnemyZombie" || app->entityManager->entities[i]->name == "EnemyShadow" || app->entityManager->entities[i]->name == "item") {
			entNode.append_attribute("num").set_value(app->entityManager->entities[i]->Num);
		}*/
		/*if (app->entityManager->entities[i]->name == "EnemyZombie") {
			entNode.append_attribute("num").set_value(app->entityManager->entities[i]->Num);
		}*/
	}

	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}