#ifndef DOTCIRCLE_H
#define DOTCIRCLE_H

#include "SDL.h"

#include "LGlobals.h"
#include "math.h"
#include "LTexture.h"

class DotCircle {
public:

	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	static const int DOT_VEL = 70;

	DotCircle(float x, float y, LTexture* texture);
	
	void handleEvent(SDL_Event& e);
	void move(float frameTime, SDL_Rect& wall, Circle& circle);
	void render();

	Circle& getCollider() { return mCollider; }

protected:
	float mPosX, mPosY;
	float mVelX, mVelY;

	Circle mCollider;
	LTexture* mTexture;

	void shiftColliders();
	bool checkCollisions(SDL_Rect& wall, Circle& circle);
};


class DotCircleAnt : public DotCircle
{
public:
	DotCircleAnt(float x, float y, LTexture* texture) : DotCircle(x, y, texture) {}
	
	void move(float frameTime, SDL_Rect& wall, Circle& circlePlayer, Circle& circleStatic);

	bool mDead = false;
	void update();

	static const int DOT_VEL = 70;

private: 
	bool checkCollisions(SDL_Rect& wall, Circle& circlePlayer, Circle& circleStatic);
};

#endif // !DOTRECT_H
