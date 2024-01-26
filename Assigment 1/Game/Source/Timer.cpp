// ----------------------------------------------------
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Timer.h"
#include "SDL\include\SDL_timer.h"
	
Timer::Timer()
{
	Start();
}

void Timer::Start()
{
    startTime = SDL_GetTicks();
    running = true;
}

uint32 Timer::ReadSec() const
{
    if (running)
        return (SDL_GetTicks() - startTime) / 1000;
    else
        return stoppedTime / 1000;
}

uint32 Timer::Stop()
{
    if (running)
    {
        running = false;
        stoppedTime = SDL_GetTicks() - startTime;
        return stoppedTime / 1000;
    }
    else
    {
        return stoppedTime / 1000;
    }
}

void Timer::Resume()
{
    if (!running)
    {
        startTime = SDL_GetTicks() - stoppedTime;
        running = true;
    }
}

float Timer::ReadMSec() const
{
	return (float)(SDL_GetTicks() - startTime);
}