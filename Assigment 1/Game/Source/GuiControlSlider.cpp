#include "GuiControlSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "Textures.h"

GuiControlSlider::GuiControlSlider(uint32 id, SDL_Rect bounds, SDL_Rect sliderBounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	this->sliderBounds = sliderBounds;

	canClick = true;
	drawBasic = false;
	
}

GuiControlSlider::~GuiControlSlider()
{

}

bool GuiControlSlider::Start()
{
	texture = app->tex->Load("Assets/Textures/UI/GUI.png");
	MusicBarRect = { 14,0,30,3 };
	MusicSliderRect = { 14,5,11,8 };

	cursorBounds = { bounds.x + (bounds.w / 2), bounds.y - 10 + 10, 22, 16 };
	return true;
}

bool GuiControlSlider::Update(float dt)
{
	
	if (state != GuiControlState::DISABLED)
	{
		app->input->GetMousePosition(mouseX, mouseY);

		if (mouseX > cursorBounds.x && mouseX < cursorBounds.x + cursorBounds.w && mouseY > cursorBounds.y && mouseY < cursorBounds.y + cursorBounds.h) {
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && mouseX >= bounds.x+5 && mouseX <= bounds.x + 115) {
				// Ajustar el valor del slider según la posición del ratón
				state = GuiControlState::PRESSED;
				//ejusta el volumen de la música y los fx
				Mix_VolumeMusic((int)sliderValue);
				Mix_Volume(-1, (int)sliderValue);
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				// Notificar a los observadores al soltar el botón del ratón
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

		//L15: DONE 4: Draw the button according the GuiControl State

		app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);
		DrawSlider();
	}

	return false;
}

void GuiControlSlider::DrawSlider()
{
	//app->render->DrawRectangle(bounds, 255, 255, 255, 180, true, false);
	SDL_RenderCopy(app->render->renderer, texture, &MusicBarRect, &bounds);

	 // Ajusta según sea necesario
	//ajusta el valor sliderValue entre 0 y 128 teniendo en cuenta el tamaño del slider
	sliderValue = (mouseX - bounds.x) * 128 / 123;

	if (state == GuiControlState::PRESSED)
	{
		cursorBounds = { mouseX-10, bounds.y, 22, 16 };
	}

	switch (state)
	{
	case GuiControlState::DISABLED:
		app->render->DrawRectangle(cursorBounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		SDL_RenderCopy(app->render->renderer, texture, &MusicSliderRect, &cursorBounds);
		/*app->render->DrawRectangle(cursorBounds, 0, 0, 255, 255, true, false);*/
		break;
	case GuiControlState::FOCUSED:
		SDL_RenderCopy(app->render->renderer, texture, &MusicSliderRect, &cursorBounds);
		break;
	case GuiControlState::PRESSED:
		SDL_RenderCopy(app->render->renderer, texture, &MusicSliderRect, &cursorBounds);
		break;
	}
}

