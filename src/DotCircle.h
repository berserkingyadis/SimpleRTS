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

protected:
	int mPosX, mPosY;
	int mVelX, mVelY;

	Circle mCollider;
	LTexture* mTexture;

	void shiftColliders();
	bool checkCollisions(SDL_Rect& wall, Circle& circle);
};


class DotCircleAnt : public DotCircle
{
public:
	DotCircleAnt(int x, int y, LTexture* texture) : DotCircle(x, y, texture) {}
	
	void move(SDL_Rect& wall, Circle& circlePlayer, Circle& circleStatic);

	bool mDead = false;
	void update();

private: 
	bool checkCollisions(SDL_Rect& wall, Circle& circlePlayer, Circle& circleStatic);
};

#endif // !DOTRECT_H
