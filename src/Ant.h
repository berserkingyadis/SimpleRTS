#ifndef ANT_H
#define ANT_H

#include <vector>

#include "CircleEntity.h"
#include "Vector2.h"

class Ant : public CircleEntity
{
public:
	Ant(float x, float y, LTexture* texture, LTexture* selTexture) : CircleEntity(x, y, texture, selTexture) {}

	void move(float frameTime, SDL_Rect& wall, Circle& circlePlayer, const std::vector<Ant*>& ants);

	bool mDead = false;
	void updateRandomly();

	void setDestination(Vector2& destination);
	void proceedToDestination(float frameTime);

	static const int DOT_VEL = 70;

private:
	bool mDestReached = true;
	Vector2 mDestinaton;
	bool checkCollisions(SDL_Rect& wall, Circle& circlePlayer, const std::vector<Ant*>& ants);
};


#endif // !ANT_H
