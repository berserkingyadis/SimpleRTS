#include "Ant.h"
#include "Collision.h"



void Ant::move(float frameTime, SDL_Rect& wall, Circle& circlePlayer, const std::vector<Ant*>& ants)
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
	if (checkCollisions(wall, circlePlayer, ants)) {
		mPosX -= mVelX * frameTime;
		shiftColliders();
	}

	mPosY += mVelY * frameTime;
	shiftColliders();
	//up and down
	if (checkCollisions(wall, circlePlayer, ants)) {
		mPosY -= mVelY * frameTime;
		shiftColliders();
	}
}

void Ant::updateRandomly()
{
	int i = rand() % 100;
	int j = rand() % 100;
	mVelX = (i < 50) ? DOT_VEL : -DOT_VEL;
	mVelY = (j < 50) ? DOT_VEL : -DOT_VEL;
}


void Ant::render() {

	if (!mDestReached) {
		SDL_SetRenderDrawColor(mTexture->getRenderer(), 240, 20, 30, SDL_ALPHA_OPAQUE);
		float beforeX = mPosX;
		float beforeY = mPosY;
		for (auto a : mMoveOrders) {
			SDL_RenderDrawLine(mTexture->getRenderer(), beforeX, beforeY, a.mX, a.mY);
			beforeX = a.mX;
			beforeY = a.mY;
		}
		
	}
	CircleEntity::render(mRotation);
	if (mSelected)mSelTexture->render(mPosX - mCollider.r, mPosY - mCollider.r,NULL,mDestinaton.angle());

	
}
void Ant::setDestination(Vector2& destination)
{
	mMoveOrders.push_back(destination);
	mDestReached = false;
	
}

void Ant::proceedToDestination(float frameTime, SDL_Rect& wall, Circle& circlePlayer, const std::vector<Ant*>& ants)
{
	if (mDestReached == false) {

		//backup old position incase we need to rollback (collision)
		float oldX = mPosX, oldY = mPosY, oldRotation = mRotation;

		//how far will we travel
		float moveDistance = frameTime * DOT_VEL;
		start:
		//how far away is the destination
		Vector2 direction = mMoveOrders.at(0) - Vector2(mPosX, mPosY);
		float destDistance = direction.length();

		//if we arrive at the target in this tick
		if (moveDistance >= destDistance) {
			mPosX = mMoveOrders.at(0).mX;
			mPosY = mMoveOrders.at(0).mY;

			//if this was the last moveorder we are finished
			if (mMoveOrders.size() == 1) {
				mMoveOrders.clear();
				mDestReached = true;
				return;
			}

			mMoveOrders.erase(mMoveOrders.begin());

			moveDistance -= destDistance;
			goto start;
		}

		Vector2 newPos = Vector2(mPosX, mPosY) + (direction.normalize() * moveDistance);
		mPosX = newPos.mX;
		mPosY = newPos.mY;
		mRotation = direction.angle();

		shiftColliders();
		if (checkCollisions(wall, circlePlayer, ants)) {
			mPosX = oldX;
			mPosY = oldY;
			mRotation = oldRotation;
			shiftColliders();
		}
	}
}


bool Ant::checkCollisions(SDL_Rect& wall, Circle& circlePlayer, const std::vector<Ant*>& ants)
{
	if (CircleEntity::checkCollisions(wall) || checkCollision(mCollider, circlePlayer) ) {
		return true;
	}
	return false;
}