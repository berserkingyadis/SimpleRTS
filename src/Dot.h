#ifndef DOT_H
#define DOT_H

#include "SDL.h"

#include "LTexture.h"
#include "LGlobals.h"

class Dot
{
public: 

	// Dimensions
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	// maximum axis velocity
	static const int DOT_VEL = 10;

	Dot(LTexture* texture, SDL_Renderer* renderer);

	void handleEvent(SDL_Event& e);

	void move(SDL_Rect& wall);
	void render();

private:

	//positions
	int mPosX, mPosY;
	int mVelX, mVelY;

	LTexture* mTexture;
	SDL_Renderer* mRenderer;

	SDL_Rect mCollider;
};

#endif