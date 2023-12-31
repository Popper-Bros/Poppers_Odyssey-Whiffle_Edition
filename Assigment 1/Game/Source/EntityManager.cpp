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

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	// Iterates over the entities and calls the Awake
	ListItem<Entity*>* item = entities.start;
	Entity* pEntity = nullptr;

	while (item != nullptr && ret == true)
	{
		pEntity = item->data;

		if (pEntity->active == false) {
			item = item->next;
			continue;
		}

		ret = pEntity->Awake();
		item = item->next;
	}

	return ret;
}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	//Creates the entity depending on the type
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::PARTICULAS:
		entity = new Particulas();
		break;
	case EntityType::ENEMYSHADOW:
		entity = new EnemyShadow();
		break;
	case EntityType::ENEMYZOMBIE:
		entity = new EnemyZombie();
		break;
	case EntityType::HEAL:
		entity = new Heal();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}