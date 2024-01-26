#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Scene.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
    // Solo procesa interacciones si el botón no está deshabilitado
    if (state != GuiControlState::DISABLED)
    {
        app->input->GetMousePosition(mouseX, mouseY);

        // Si el ratón está dentro de los límites del botón
        if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h)
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                if (id == 1)
                {
                    app->scene->isPaused = false;
                }  
                if (id == 4)
                {
                    app->scene->esc = true;
                }
            }

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
            {
                NotifyObserver();
            }
        }
        else
        {
            state = GuiControlState::NORMAL;
        }

        // Draw the button according to the GuiControl State
       
    }
    else
    {
        if (!app->scene->isPaused)
        {
            state = GuiControlState::DISABLED;
        }
    }

    // Draw the button with transparency even when disabled
    switch (state)
    {
    case GuiControlState::DISABLED:
        app->render->DrawRectangle(bounds, 0, 0, 0, 0, true, false); // Transparente
        break;
    case GuiControlState::NORMAL:
        app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
        break;
    case GuiControlState::FOCUSED:
        app->render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
        break;
    case GuiControlState::PRESSED:
        app->render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
        break;
    }
    app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);

    return false;
}


void GuiControlButton::DestroyButton() {
	
	//delete the button
	

}