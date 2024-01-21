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
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				// Ajustar el valor del slider según la posición del ratón
				float normalizedX = static_cast<float>(mouseX - bounds.x) / static_cast<float>(bounds.w);
				currentValue = minValue + normalizedX * (maxValue - minValue);
				state = GuiControlState::PRESSED;
				DrawSlider();
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

	}

	return false;
}

void GuiControlSlider::DrawSlider()
{
	app->render->DrawRectangle(bounds, 255, 255, 255, 255, true, false);

	float normalizedPosition = (currentValue - minValue) / (maxValue - minValue);
	int cursorX = bounds.x + static_cast<int>(normalizedPosition * bounds.w);

	SDL_Rect cursorBounds = { cursorX - 5, bounds.y + 10, 5, 5 }; // Ajusta según sea necesario

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

