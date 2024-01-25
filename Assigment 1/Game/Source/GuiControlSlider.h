#ifndef __GUICONTROLSLIDER_H__
#define __GUICONTROLSLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlSlider : public GuiControl
{

public:

	GuiControlSlider(uint32 id, SDL_Rect bounds, SDL_Rect sliderBounds, const char* text);
	virtual ~GuiControlSlider();

	bool Start();
	// Called each loop iteration
	bool Update(float dt);

	void DrawSlider();

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
	float minValue;
	float maxValue;
	float currentValue;
	float sliderValue;

	SDL_Rect cursorBounds;
	iPoint position;

	const char* GUIpath;

	SDL_Texture* GUITex;

	SDL_Rect MusicBarRect;
	SDL_Rect MusicSliderRect;


};

#endif // __GUICONTROLSLIDER_H__