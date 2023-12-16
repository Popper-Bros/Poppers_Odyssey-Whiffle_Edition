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
	if (config.child("item")) {
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = config.child("item");
		item->Num = 1;
	}

	if (config.child("item2")) {
		item2 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item2->parameters = config.child("item2");
		item2->Num = 2;
	}

	if (config.child("item3")) {
		item3 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item3->parameters = config.child("item3");
		item3->Num = 3;
	}

	if (config.child("item4")) {
		item4 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item4->parameters = config.child("item4");
		item4->Num = 4;
	}

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	if (config.child("Particulas")) {
		particulas = (Particulas*)app->entityManager->CreateEntity(EntityType::PARTICULAS);
		particulas->parameters = config.child("Particulas");
	}

	if (config.child("EnemyShadow")) {
		enemy = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
		enemy->parameters = config.child("EnemyShadow");
		enemy->Num = 1;
	}

	if (config.child("EnemyShadow2")) {
		enemy3 = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
		enemy3->parameters = config.child("EnemyShadow2");
		enemy3->Num = 2;
	}

	if (config.child("EnemyZombie")) {
		enemy2 = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
		enemy2->parameters = config.child("EnemyZombie");
		enemy2->Num = 1;
	}

	if (config.child("EnemyZombie2")) {
		enemy4 = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
		enemy4->parameters = config.child("EnemyZombie2");
		enemy4->Num = 2;
	}

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

	//camera movement
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

	//el player vuelve al pricipio 
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && player->isAlive && player->fell == false) { 
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0); 
		checkpoint == 0;
	}
	//el player vuelve al checkpoint
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && player->isAlive && player->fell == false) {
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(980),PIXEL_TO_METERS(150) }, 0);
		checkpoint == 1;
	}
	//el player respawnea en el ultimo checkpoint
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && player->isAlive && player->fell == false){
		if (checkpoint==0) player->pbody->body->SetTransform({ PIXEL_TO_METERS(80),PIXEL_TO_METERS(182) }, 0);
		else if (checkpoint == 1) player->pbody->body->SetTransform({ PIXEL_TO_METERS(980),PIXEL_TO_METERS(150) }, 0);
	}

	playerTile = app->map->WorldToMap(25+player->position.x, player->position.y - app->render->camera.y);
		
	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)

	//hace un save
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	//hace un load
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

bool Scene::LoadState(pugi::xml_node node) // esta funcion carga los datos del xml en las variables de los objetos
{

	//for (pugi::xml_node itemNode = node.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	//{
	//	item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	//	item->parameters = itemNode;
	//}
	
	
	if (node.child("Player")) {
		player->position.x = node.child("Player").attribute("x").as_int();
		player->position.y = node.child("Player").attribute("y").as_int();
		player->intoxication = node.child("Player").attribute("intoxication").as_int();
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(player->position.x+0.3f),PIXEL_TO_METERS(player->position.y+0.3f) }, 0);
	}

	for (pugi::xml_node enemyShadowNode = node.child("EnemyShadow"); enemyShadowNode; enemyShadowNode = enemyShadowNode.next_sibling("EnemyShadow")) {

		if (enemyShadowNode.attribute("num").as_int()==1) {
			if (enemyShadowNode.attribute("isAlive").as_bool() == true && enemy->isAlive == false) {
				enemy = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
				enemy->Awake();
				enemy->texturepath = enemyShadowNode.attribute("texturepath").as_string();
				enemy->Start();
				enemy->isAlive = true;
			}
			if (enemyShadowNode.attribute("isAlive").as_bool() == false && enemy->isAlive == true) {
				enemy->isAlive = false;
			}
			enemy->position.x = enemyShadowNode.attribute("x").as_int();
			enemy->position.y = enemyShadowNode.attribute("y").as_int();

			enemy->pbody->body->SetTransform({ PIXEL_TO_METERS(enemy->position.x+0.4f),PIXEL_TO_METERS(enemy->position.y+0.73f) }, 0);

		}

		if (enemyShadowNode.attribute("num").as_int() == 2) {
			if (enemyShadowNode.attribute("isAlive").as_bool() == true && enemy3->isAlive == false) {
				enemy3 = (EnemyShadow*)app->entityManager->CreateEntity(EntityType::ENEMYSHADOW);
				enemy3->Awake();
				enemy3->texturepath = enemyShadowNode.attribute("texturepath").as_string();
				enemy3->Start();
				enemy3->isAlive = true;
			}
			if (enemyShadowNode.attribute("isAlive").as_bool() == false && enemy3->isAlive == true) {
				enemy3->isAlive = false;
			}
			enemy3->position.x = enemyShadowNode.attribute("x").as_int();
			enemy3->position.y = enemyShadowNode.attribute("y").as_int();

			enemy3->pbody->body->SetTransform({ PIXEL_TO_METERS(enemy3->position.x + 0.4f),PIXEL_TO_METERS(enemy3->position.y + 0.73f) }, 0);
		}
	}

	for (pugi::xml_node enemyZombieNode = node.child("EnemyZombie"); enemyZombieNode; enemyZombieNode = enemyZombieNode.next_sibling("EnemyZombie")) {

		if (enemyZombieNode.attribute("num").as_int() == 1) {
			if (enemyZombieNode.attribute("isAlive").as_bool() == true && enemy2->isAlive == false) {
				enemy2 = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
				enemy2->Awake();
				enemy2->texturepath = enemyZombieNode.attribute("texturepath").as_string();
				enemy2->Start();
				enemy2->isAlive = true;
			}
			if (enemyZombieNode.attribute("isAlive").as_bool() == false && enemy2->isAlive == true) {
				enemy2->isAlive = false;
			}
			enemy2->position.x = enemyZombieNode.attribute("x").as_int();
			enemy2->position.y = enemyZombieNode.attribute("y").as_int();

			enemy2->pbody->body->SetTransform({ PIXEL_TO_METERS(enemy2->position.x + 0.6f),PIXEL_TO_METERS(enemy2->position.y + 0.93f) }, 0);
		}

		if (enemyZombieNode.attribute("num").as_int() == 2) {
			if (enemyZombieNode.attribute("isAlive").as_bool() == true && enemy4->isAlive == false) {
				enemy4 = (EnemyZombie*)app->entityManager->CreateEntity(EntityType::ENEMYZOMBIE);
				enemy4->Awake();
				enemy4->texturepath = enemyZombieNode.attribute("texturepath").as_string();
				enemy4->Start();
				enemy4->isAlive = true;
			}
			if (enemyZombieNode.attribute("isAlive").as_bool() == false && enemy4->isAlive == true) {
				enemy4->isAlive = false;
			}
			enemy4->position.x = enemyZombieNode.attribute("x").as_int();
			enemy4->position.y = enemyZombieNode.attribute("y").as_int();

			enemy4->pbody->body->SetTransform({ PIXEL_TO_METERS(enemy4->position.x + 0.6f),PIXEL_TO_METERS(enemy4->position.y + 0.93f) }, 0);
		}
	}

	for (pugi::xml_node itemNode = node.child("item"); itemNode; itemNode = itemNode.next_sibling("item")) {
		if (itemNode.attribute("num").as_int() == 1) {
			if (itemNode.attribute("isAlive").as_bool() == true && item->isAlive == false) {
				item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
				item->Awake();
				item->texturepath = itemNode.attribute("texturepath").as_string();
				item->Start();
				item->isAlive = true;
			}
			if (itemNode.attribute("isAlive").as_bool() == false && item->isAlive == true) {
				item->isAlive = false;
			}
			item->position.x = itemNode.attribute("x").as_int();
			item->position.y = itemNode.attribute("y").as_int();

			item->pbody->body->SetTransform({ PIXEL_TO_METERS(item->position.x),PIXEL_TO_METERS(item->position.y) }, 0);
		}
		if (itemNode.attribute("num").as_int() == 2) {
			if (itemNode.attribute("isAlive").as_bool() == true && item2->isAlive == false) {
				item2 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
				item2->Awake();
				item2->texturepath = itemNode.attribute("texturepath").as_string();
				item2->Start();
				item2->isAlive = true;
			}
			if (itemNode.attribute("isAlive").as_bool() == false && item2->isAlive == true) {
				item2->isAlive = false;
			}
			item2->position.x = itemNode.attribute("x").as_int();
			item2->position.y = itemNode.attribute("y").as_int();

			item2->pbody->body->SetTransform({ PIXEL_TO_METERS(item2->position.x),PIXEL_TO_METERS(item2->position.y) }, 0);
		}
		if (itemNode.attribute("num").as_int() == 3) {
			if (itemNode.attribute("isAlive").as_bool() == true && item3->isAlive == false) {
				item3 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
				item3->Awake();
				item3->texturepath = itemNode.attribute("texturepath").as_string();
				item3->Start();
				item3->isAlive = true;
			}
			if (itemNode.attribute("isAlive").as_bool() == false && item3->isAlive == true) {
				item3->isAlive = false;
			}
			item3->position.x = itemNode.attribute("x").as_int();
			item3->position.y = itemNode.attribute("y").as_int();

			item3->pbody->body->SetTransform({ PIXEL_TO_METERS(item3->position.x),PIXEL_TO_METERS(item3->position.y) }, 0);
		}
		if (itemNode.attribute("num").as_int() == 4) {
			if (itemNode.attribute("isAlive").as_bool() == true && item4->isAlive == false) {
				item4 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
				item4->Awake();
				item4->texturepath = itemNode.attribute("texturepath").as_string();
				item4->Start();
				item4->isAlive = true;
			}
			if (itemNode.attribute("isAlive").as_bool() == false && item4->isAlive == true) {
				item4->isAlive = false;
			}
			item4->position.x = itemNode.attribute("x").as_int();
			item4->position.y = itemNode.attribute("y").as_int();

			item4->pbody->body->SetTransform({ PIXEL_TO_METERS(item4->position.x),PIXEL_TO_METERS(item4->position.y) }, 0);
		}
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
		}
		if (app->entityManager->entities[i]->name == "EnemyZombie" || app->entityManager->entities[i]->name == "EnemyShadow" || app->entityManager->entities[i]->name == "item") {
			entNode.append_attribute("num").set_value(app->entityManager->entities[i]->Num);
		}
		/*if (app->entityManager->entities[i]->name == "EnemyZombie") {
			entNode.append_attribute("num").set_value(app->entityManager->entities[i]->Num);
		}*/
	}

	return true;
}