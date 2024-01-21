#include "GuiControlSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

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

bool GuiControlSlider::Update(float dt)
{
	cursorBounds = { bounds.x + (bounds.w / 2), bounds.y + 10, 5, 5 };
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		if (mouseX > cursorBounds.x && mouseX < cursorBounds.x + cursorBounds.w && mouseY > cursorBounds.y && mouseY < cursorBounds.y + cursorBounds.h) {
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				// Ajustar el valor del slider según la posición del ratón
				state = GuiControlState::PRESSED;
				
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
	app->render->DrawRectangle(bounds, 255, 255, 255, 100, true, false);

	float normalizedPosition = (currentValue - minValue) / (maxValue - minValue);
	int cursorX = bounds.x + static_cast<int>(normalizedPosition * bounds.w);

	 // Ajusta según sea necesario

	if (state == GuiControlState::PRESSED)
	{
		cursorBounds = { mouseX, bounds.y + 10, 5, 5 };
	}

	switch (state)
	{
	case GuiControlState::DISABLED:
		app->render->DrawRectangle(cursorBounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		app->render->DrawRectangle(cursorBounds, 0, 0, 255, 255, true, false);
		break;
	case GuiControlState::FOCUSED:
		app->render->DrawRectangle(cursorBounds, 0, 0, 20, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		app->render->DrawRectangle(cursorBounds, 255, 0, 0, 255, true, false);
		break;
	}


	// Dibujar el "cursor" del slider
	

}

