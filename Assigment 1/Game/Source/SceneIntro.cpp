#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneIntro.h"
#include "Scene.h"
#include "Pathfinding.h"
#include "Defs.h"
#include "Log.h"
#include "HUD.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Optick/include/optick.h"

SceneIntro::SceneIntro() : Module()
{
	name.Create("SceneIntro");
}

// Destructor
SceneIntro::~SceneIntro()
{}

// Called before render is available
bool SceneIntro::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");

	//characters intro
	title.PushBack({ 0, 0, 1280, 720 });
	title.PushBack({ 1280, 0, 1280, 720 });
	title.PushBack({ 2560, 0, 1280, 720 });
	title.PushBack({ 3840, 0, 1280, 720 });
	title.PushBack({ 5120, 0, 1280, 720 });

	title.PushBack({ 0, 720, 1280, 720 });
	title.PushBack({ 1280, 720, 1280, 720 });
	title.PushBack({ 2560, 720, 1280, 720 });
	title.PushBack({ 3840, 720, 1280, 720 });
	title.PushBack({ 5120, 720, 1280, 720 });

	title.PushBack({ 0, 1440, 1280, 720 });
	title.PushBack({ 1280, 1440, 1280, 720 });
	title.PushBack({ 2560, 1440, 1280, 720 });
	title.PushBack({ 3840, 1440, 1280, 720 });
	title.PushBack({ 5120, 1440, 1280, 720 });

	title.PushBack({ 0, 2160, 1280, 720 });
	title.PushBack({ 1280, 2160, 1280, 720 });
	title.PushBack({ 2560, 2160, 1280, 720 });
	title.PushBack({ 3840, 2160, 1280, 720 });
	title.PushBack({ 5120, 2160, 1280, 720 });

	title.PushBack({ 0, 2880, 1280, 720 });
	title.PushBack({ 1280, 2880, 1280, 720 });
	title.PushBack({ 2560, 2880, 1280, 720 });
	title.PushBack({ 3840, 2880, 1280, 720 });
	title.PushBack({ 5120, 2880, 1280, 720 });

	title.PushBack({ 0, 3600, 1280, 720 });
	title.PushBack({ 1280, 3600, 1280, 720 });
	title.PushBack({ 2560, 3600, 1280, 720 });
	title.PushBack({ 3840, 3600, 1280, 720 });
	title.PushBack({ 5120, 3600, 1280, 720 });

	title.PushBack({ 0, 4320, 1280, 720 });
	title.PushBack({ 1280, 4320, 1280, 720 });
	title.PushBack({ 2560, 4320, 1280, 720 });
	title.PushBack({ 3840, 4320, 1280, 720 });
	title.PushBack({ 5120, 4320, 1280, 720 });


	title.PushBack({ 5120, 4320, 1280, 720 });
	title.PushBack({ 5120, 4320, 1280, 720 });
	title.PushBack({ 5120, 4320, 1280, 720 });
	title.PushBack({ 5120, 4320, 1280, 720 });
	title.PushBack({ 5120, 4320, 1280, 720 });

	title.loop = false;
	title.speed = 0.01f * 16;

	bool ret = true;

	return ret;
}
// Called before the first frame
bool SceneIntro::Start()
{
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music.ogg");

	introTexture = app->tex->Load("Assets/Textures/Intro/intro.png");
	
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);
	
	//Get the size of the texture
	app->tex->GetSize(introTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(introTexture, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SDL_Rect btPos = { windowW / 2 - 60,20, 120,12 };
	//gcButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Didac, el virgen", btPos, this);
	//gcButton = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "", btPos, this);

	return true;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneIntro::Update(float dt)
{
	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(introTexture, 0, 0, &rect);
	return true;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	if (currentAnimation->GetCurrentFrameIndex() >= 39) {
		CleanUp();
	}

	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneIntro::CleanUp()
{
	LOG("Freeing SceneIntro");
	app->tex->UnLoad(introTexture);
	//app->audio->CleanUp();

	//app->audio->Awake(app->scene->savedConfig);

	return true;
}

bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}