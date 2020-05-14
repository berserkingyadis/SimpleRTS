#ifndef LTIMER_H
#define LTIMER_H

#include "SDL.h"

class LTimer {
public:
	LTimer();

	void start();
	void stop();
	void pause();
	void unpause();

	uint32_t getTicks();

	bool isStarted() { return mStarted; }
	bool isPaused() { return mPaused; }
private:

	uint32_t mStartTicks;
	uint32_t mPausedTicks;

	bool mPaused;
	bool mStarted;

	void reset();
};

#endif