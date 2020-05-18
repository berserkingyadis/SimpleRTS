#include "LTimer.h"

LTimer::LTimer()
{
	reset();
}

void LTimer::start()
{
	mStarted = true;
	mPaused = false;
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
	reset();
}

void LTimer::pause()
{
	if (mStarted && !mPaused) {
		mPaused = true;
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	if (mStarted && mPaused) {
		mPaused = false;
		mStartTicks = SDL_GetTicks() - mPausedTicks;
		mPausedTicks = 0;
	}
}

uint32_t LTimer::getTicks()
{
	uint32_t time = 0;

	if (mStarted) {
		if (mPaused) {
			time = mPausedTicks;
		}
		else {
			time = SDL_GetTicks() - mStartTicks;
		}
	}
	return time;
}

void LTimer::reset()
{
	mStartTicks = 0;
	mPausedTicks = 0;
	mPaused = false;
	mStarted = false;
}
