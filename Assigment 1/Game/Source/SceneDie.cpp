#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneDie.h"
#include "Scene.h"
#include "Pathfinding.h"
#include "Defs.h"
#include "Log.h"
#include "HUD.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Optick/include/optick.h"

SceneDie::SceneDie() : Module()
{
	name.Create("SceneDie");
}

// Destructor
SceneDie::~SceneDie()
{}

// Called before render is available
bool SceneDie::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneDie");

	bool ret = true;

	return ret;
}
// Called before the first frame
bool SceneDie::Start()
{
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music.ogg");

	
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);
	
	//Get the size of the texture
	app->tex->GetSize(dieTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(dieTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SDL_Rect btPos = { windowW / 2 - 60,20, 120,12 };
	//gcButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Didac, el virgen", btPos, this);
	//gcButton = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "", btPos, this);

	return true;
}

// Called each loop iteration
bool SceneDie::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneDie::Update(float dt)
{


	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) {

		app->scene->player->isAlive = true;
		app->scene->player->reset = false;
		app->scene->player->currentAnimation->Reset();
		app->LoadRequest();
		CleanUp();
	}

	if (app->scene->player->Die.GetCurrentFrameIndex() >= 7) {
		dieTexture = app->tex->Load("Assets/Textures/rataTiesa.png");
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = windowW;
		rect.h = windowH;
		app->render->DrawTexture(dieTexture, 0, 0, &rect);
	}

	return true;
}

// Called each loop iteration
bool SceneDie::PostUpdate()
{

	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneDie::CleanUp()
{
	LOG("Freeing SceneDie");
	app->tex->UnLoad(dieTexture);
	//app->audio->CleanUp();

	//app->audio->Awake(app->scene->savedConfig);

	return true;
}

bool SceneDie::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}