#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneMenu.h"
#include "Scene.h"
#include "Pathfinding.h"
#include "Defs.h"
#include "Log.h"
#include "HUD.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Optick/include/optick.h"

SceneMenu::SceneMenu() : Module()
{
	name.Create("SceneMenu");
}

// Destructor
SceneMenu::~SceneMenu()
{}

// Called before render is available
bool SceneMenu::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneMenu");

	bool ret = true;

	return ret;
}
// Called before the first frame
bool SceneMenu::Start()
{
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music.ogg");

	menuTexture = app->tex->Load("Assets/Textures/menu.png");
	
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);
	
	//Get the size of the texture
	app->tex->GetSize(menuTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(menuTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SDL_Rect btPos = { windowW / 2 - 60,20, 120,12 };
	//gcButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Didac, el virgen", btPos, this);
	//gcButton = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "", btPos, this);

	return true;
}

// Called each loop iteration
bool SceneMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMenu::Update(float dt)
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = windowW;
	rect.h = windowH;
	app->render->DrawTexture(menuTexture, 0, 0, &rect);
	return true;
}

// Called each loop iteration
bool SceneMenu::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT) {
		CleanUp();
	}

	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneMenu::CleanUp()
{
	LOG("Freeing SceneMenu");
	app->tex->UnLoad(menuTexture);
	//app->audio->CleanUp();

	//app->audio->Awake(app->scene->savedConfig);

	return true;
}

bool SceneMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}