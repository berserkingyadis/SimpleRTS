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



void Ant::setDestination(Vector2& destination)
{
	mDestinaton = destination;
	mDestReached = false;
}

void Ant::proceedToDestination(float frameTime, SDL_Rect& wall, Circle& circlePlayer, const std::vector<Ant*>& ants)
{
	if (mDestReached == false) {
		float oldX = mPosX, oldY = mPosY;
		//how far away is the destination
		Vector2 direction = mDestinaton - Vector2(mPosX, mPosY);
		float destDistance = direction.length();
		if (destDistance == 0) {
			mDestReached = true;
			return;
		}
		//how far will we travel
		float howFar = frameTime * DOT_VEL;

		Vector2 newPos = Vector2(mPosX, mPosY) + (direction.normalize() * howFar);

		//check for collisions

		mPosX = newPos.mX;
		mPosY = newPos.mY;
		shiftColliders();
		if (checkCollisions(wall, circlePlayer, ants)) {
			mPosX = oldX;
			mPosY = oldY;
			shiftColliders();
		}
			
	}
	
}


bool Ant::checkCollisions(SDL_Rect& wall, Circle& circlePlayer, const std::vector<Ant*>& ants)
{
	if (CircleEntity::checkCollisions(wall) || checkCollision(mCollider, circlePlayer) || checkCollision(this, ants) ) {
		return true;
	}
	return false;
}