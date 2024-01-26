#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"
#include "HUD.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "Animation.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro();

	// Destructor
	virtual ~SceneIntro();

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

	Animation* currentAnimation = &title;

	//Animation idleAnim;
	Animation title;

	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	SDL_Texture* introTexture;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	//GuiControlButton* gcButton;
	GuiControlSlider* Slider;
	GuiControlButton* gcButton;


};

#endif // __SceneIntro_H__