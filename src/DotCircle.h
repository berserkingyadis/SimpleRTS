#ifndef DOTCIRCLE_H
#define DOTCIRCLE_H

#include "SDL.h"

#include "LGlobals.h"
#include "LTexture.h"

class DotCircle {
public:

	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	static const int DOT_VEL = 1;

	DotCircle(int x, int y, LTexture* texture);
	
	void handleEvent(SDL_Event& e);

	void move(SDL_Rect& wall, Circle& circle);

	void render();

	Circle& getCollider() { return mCollider; }


private:
	int mPosX, mPosY;
	int mVelX, mVelY;

	Circle mCollider;

	LTexture* mTexture;

	void shiftColliders();
};



#endif // !DOTRECT_H
