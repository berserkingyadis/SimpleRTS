#include "CircleEntity.h"

#include "Collision.h"


CircleEntity::CircleEntity(float x, float y, LTexture* texture)
{
	mPosX = x;
	mPosY = y;
	mVelX = 0;
	mVelY = 0;

	mTexture = texture;

	mCollider.r = DOT_WIDTH / 2;
	shiftColliders();
}

void CircleEntity::handleEvent(SDL_Event& e)
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

void CircleEntity::move(float frameTime, SDL_Rect& wall, Circle& circle)
{
	mPosX += mVelX * frameTime;
	shiftColliders();

	/*
		We need to check the following collisions:
		1) Does the dot leave the screen?
		2) Does the dot collide with the other dot?
		3) Does the dot collider with the wall?
	*/

	// left and right
	if (checkCollisions(wall, circle)){
		mPosX -= mVelX * frameTime;
	}

	mPosY += mVelY * frameTime;
	shiftColliders();
	//up and down
	if (checkCollisions(wall, circle)) {
		mPosY -= mVelY * frameTime;
		shiftColliders();
	}
}

void CircleEntity::render()
{
	mTexture->render(mPosX - mCollider.r, mPosY - mCollider.r);
}

void CircleEntity::shiftColliders()
{
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}

bool CircleEntity::checkCollisions(SDL_Rect& wall, Circle& circle)
{
	if(mPosX - mCollider.r < 0 || mPosX + mCollider.r > SCREEN_WIDTH
		|| mPosY - mCollider.r < 0 || mPosY + mCollider.r > SCREEN_HEIGHT
		|| checkCollision(mCollider, wall) || checkCollision(mCollider, circle))
		return true;
	return false;
}

