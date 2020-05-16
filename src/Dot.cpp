#include "Dot.h"

#include "Collision.h"

Dot::Dot(LTexture* texture, SDL_Renderer* renderer)
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
	mRenderer = renderer;
}

void Dot::handleEvent(SDL_Event& e)
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

void Dot::move(SDL_Rect& wall)
{
	mPosX += mVelX;
	mCollider.x = mPosX;
	if (mPosX < 0 || (mPosX + DOT_WIDTH > SCREEN_WIDTH)) {
		mPosX -= mVelX;
		mCollider.x = mPosX;
	}
		

	mPosY += mVelY;
	mCollider.y = mPosY;
	if (mPosY < 0 || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT)) {
		mPosY -= mVelY;
		mCollider.y = mPosY;
	}
}

void Dot::render()
{
	mTexture->render(mPosX, mPosY);
}
