#include "RectEntity.h"
#include "Collision.h"

RectEntity::RectEntity(LTexture* texture)
{
	mPosX = 0;
	mPosY = 0;
	mVelX = 0;
	mVelY = 0;
	mCollider.x = 0;
	mCollider.y = 0;
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;

	mTexture = texture;
}

void RectEntity::handleEvent(SDL_Event& e)
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

void RectEntity::move(SDL_Rect& wall)
{
	mPosX += mVelX;
	mCollider.x = mPosX;
	if (mPosX < 0 || (mPosX + DOT_WIDTH > SCREEN_WIDTH) || checkCollision(mCollider, wall)) {
		mPosX -= mVelX;
		mCollider.x = mPosX;
	}
		

	mPosY += mVelY;
	mCollider.y = mPosY;
	if (mPosY < 0 || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT) || checkCollision(mCollider, wall)) {
		mPosY -= mVelY;
		mCollider.y = mPosY;
	}
}

void RectEntity::render()
{
	mTexture->render(mPosX, mPosY);
}
