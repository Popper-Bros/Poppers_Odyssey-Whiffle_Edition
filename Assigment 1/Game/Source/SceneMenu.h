#ifndef __SCENEMENU_H__
#define __SCENEMENU_H__

#include "Module.h"
#include "HUD.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "Animation.h"

struct SDL_Texture;

class SceneMenu : public Module
{
public:

	SceneMenu();

	// Destructor
	virtual ~SceneMenu();

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
	SDL_Texture* menuTexture;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	//GuiControlButton* gcButton;
	GuiControlSlider* Slider;
	GuiControlButton* gcButton;


};

#endif // __SceneMenu_H__