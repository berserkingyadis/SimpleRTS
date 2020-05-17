#include "Collision.h"

bool checkCollision(SDL_Rect& a, SDL_Rect& b) {
	int leftA = a.x;
	int rightA = a.x + a.w;
	int topA = a.y;
	int bottomA = a.y + a.h;

	int leftB = b.x;
	int rightB = b.x + b.w;
	int topB = b.y;
	int bottomB = b.y + b.h;

	if (bottomA <= topB) return false;
	if (topA >= bottomB) return false;
	if (rightA <= leftB) return false;
	if (leftA >= rightB) return false;

	return true;
}

bool checkCollision(Circle& a, Circle& b) {

	// Check if the distance between the centers is less than the sum of the radii
	// (we used squared values here because square root operations are "expensive"
	int radiiSquared = a.r + b.r;
	radiiSquared = radiiSquared * radiiSquared;

	if (distanceSquared(a.x, a.y, b.x, b.y) < radiiSquared) return true;
	return false;;
}

/*
	To check of a box and a circle collide we need to find the closest point (cX, cY) on the box

	cX:
	if center of the circle is left of the square:
	  cX = b.x
	if center of the circle is right of the square
	  cX = b.x + b.w
	else the circle is above or below the square
	  cX = a.x

	cY:
	if center of the circle is above the square:
	  cY = b.y
	if center of the circle is below the square:
	  cY = b.y + b.h
	else the circle is on the side
	  cY = a.y

	then check if the closest point is in the circle with distance


*/
bool checkCollision(Circle& a, SDL_Rect& b) {
	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (a.x < b.x) cX = b.x;
	else if (a.x > b.x + b.w) cX = b.x + b.w;
	else cX = a.x;

	//Find closest y offset
	if (a.y < b.y) cY = b.y;
	else if (a.y > b.y + b.h) cY = b.y + b.h;
	else cY = a.y;

	if (distanceSquared(a.x, a.y, cX, cY) < a.r * a.r) return true;
	return false;
}

double distanceSquared(int x1, int y1, int x2, int y2) {
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX * deltaX + deltaY * deltaY;
}