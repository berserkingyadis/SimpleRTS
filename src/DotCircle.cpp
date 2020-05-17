#include "DotCircle.h"

#include "Collision.h"


DotCircle::DotCircle(int x, int y, LTexture* texture)
{
	mPosX = x;
	mPosY = y;

	mVelX = 0;
	mVelY = 0;

	mTexture = texture;

	mCollider.r = DOT_WIDTH / 2;
	shiftColliders();
}

void DotCircle::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_UP: mVelY -= DOT_VEL; break;
		case SDLK_DOWN: mVelY += DOT_VEL; break;
		case SDLK_LEFT: mVelX -= DOT_VEL; break;
		case SDLK_RIGHT:mVelX += DOT_VEL; break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym) {
		case SDLK_UP: mVelY += DOT_VEL; break;
		case SDLK_DOWN: mVelY -= DOT_VEL; break;
		case SDLK_LEFT: mVelX += DOT_VEL; break;
		case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}
	}
}

void DotCircle::move(SDL_Rect& wall, Circle& circle)
{
	mPosX += mVelX;
	shiftColliders();

	/*
		We need to check the following collisions:
		1) Does the dot leave the screen?
		2) Does the dot collide with the other dot?
		3) Does the dot collider with the wall?
	*/

	// left and right
	if (mPosX - mCollider.r < 0 || mPosX + mCollider.r > SCREEN_WIDTH
		|| checkCollision(mCollider, wall) || checkCollision(mCollider, circle)) {
		mPosX -= mVelX;
		shiftColliders();
	}

	mPosY += mVelY;
	shiftColliders();
	//up and down
	if (mPosY - mCollider.r < 0 || mPosY + mCollider.r > SCREEN_HEIGHT
		|| checkCollision(mCollider, wall) || checkCollision(mCollider, circle)) {
		mPosY -= mVelY;
		shiftColliders();
	}
}

void DotCircle::render()
{
	mTexture->render(mPosX - mCollider.r, mPosY - mCollider.r);
}

void DotCircle::shiftColliders()
{
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}