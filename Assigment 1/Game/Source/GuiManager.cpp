#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "Audio.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

// L15: DONE1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(id, bounds, text);
		break;
	case GuiControlType::SLIDER:
		guiControl = new GuiControlSlider(id, bounds, sliderBounds, text);
		break;
	}

	//Set the observer
	guiControl->observer = observer;

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

GuiControl* GuiManager::DestroyButton(GuiControlButton* guiControl)
{
	ListItem<GuiControl*>* control = guiControlsList.start;
	GuiControl* deletedControl = nullptr;

	while (control != nullptr)
	{
		if (control->data == guiControl)
		{
			deletedControl = control->data;
			guiControlsList.Del(control);
			break;
		}
		control = control->next;
	}

	delete deletedControl;
	return guiControl;
}

GuiControl* GuiManager::DestroySlider(GuiControlSlider* guiControl)
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		if (control->data == guiControl)
		{
			guiControlsList.Del(control);
			break;
		}
		control = control->next;
	}

	return guiControl;
}

bool GuiManager::Start()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Start();
		control = control->next;
	}

	return true;
}

bool GuiManager::Update(float dt)
{	

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Update(dt);
		control = control->next;
	}

	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}



