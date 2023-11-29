#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"
#define MAX_FRAMES 60

class Animation
{
public:
	bool loop = true;
	float speed = 0.01f;
	SDL_Rect frames[MAX_FRAMES];
	// Allows the animation to keep going back and forth
	bool pingpong = false;

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongDirection = 1;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
	}

	int GetCurrentFrameIndex()
	{
		return currentFrame;
	}

	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = -pingpongDirection;
		}
	}

	SDL_Rect& GetCurrentFrame()
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}

	void LoadAnimation(const char* n, const char* nombre) {
		pugi::xml_document configFile;
		pugi::xml_node animationsNode;
		pugi::xml_node animNode;
		pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
		animationsNode = configFile.child("config").child("scene").child(n).child("animations").child(nombre);

		animNode = animationsNode.child("anim");

		while (animNode != NULL) {
			this->PushBack({
				animNode.attribute("x").as_int(),
				animNode.attribute("y").as_int(),
				animNode.attribute("width").as_int(),
				animNode.attribute("height").as_int() });
			animNode = animNode.next_sibling();
		}
		this->loop = animationsNode.attribute("loop").as_bool();
		this->speed = animationsNode.attribute("speed").as_float();
	}
	
};

#endif