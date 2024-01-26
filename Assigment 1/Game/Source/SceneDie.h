#ifndef __SCENEDIE_H__
#define __SCENEDIE_H__

#include "Module.h"
#include "HUD.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "Animation.h"

struct SDL_Texture;

class SceneDie : public Module
{
public:

	SceneDie();

	// Destructor
	virtual ~SceneDie();

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

	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	SDL_Texture* dieTexture;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	//GuiControlButton* gcButton;
	GuiControlSlider* Slider;
	GuiControlButton* gcButton;


};

#endif // __SceneDie_H__