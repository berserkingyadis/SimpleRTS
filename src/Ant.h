#ifndef ANT_H
#define ANT_H

#include <vector>

#include "CircleEntity.h"

class Ant : public CircleEntity
{
public:
	Ant(float x, float y, LTexture* texture) : CircleEntity(x, y, texture) {}

	void move(float frameTime, SDL_Rect& wall, Circle& circlePlayer, Circle& circleStatic, const std::vector<Ant*>& ants);

	bool mDead = false;
	void update();

	static const int DOT_VEL = 70;

private:
	bool checkCollisions(SDL_Rect& wall, Circle& circlePlayer, Circle& circleStatic);
};


#endif // !ANT_H
