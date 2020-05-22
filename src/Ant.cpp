void Ant::move(float frameTime, SDL_Rect& wall, Circle& circlePlayer, Circle& circleStatic, const std::vector<Ant*>& ants)
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
	if (checkCollisions(wall, circlePlayer, circleStatic)) {
		mPosX -= mVelX * frameTime;
		shiftColliders();
	}

	mPosY += mVelY * frameTime;
	shiftColliders();
	//up and down
	if (checkCollisions(wall, circlePlayer, circleStatic)) {
		mPosY -= mVelY * frameTime;
		shiftColliders();
	}
}

void Ant::update()
{
	int i = rand() % 100;
	int j = rand() % 100;
	mVelX = (i < 50) ? DOT_VEL : -DOT_VEL;
	mVelY = (j < 50) ? DOT_VEL : -DOT_VEL;
}


bool Ant::checkCollisions(SDL_Rect& wall, Circle& circlePlayer, Circle& circleStatic)
{
	if (CircleEntity::checkCollisions(wall, circleStatic) || checkCollision(mCollider, circlePlayer)) {
		mDead = true;
		return true;
	}
	return false;
}