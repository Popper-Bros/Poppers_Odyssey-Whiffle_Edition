#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	uint32 ReadSec() const;
	uint32 Stop();
	void Resume();
	float ReadMSec() const;
	

private:
	uint32 startTime;
	bool running;
	uint32 stoppedTime;
};

#endif //__TIMER_H__