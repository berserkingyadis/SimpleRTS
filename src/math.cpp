#include "math.h"

float lerp(float v0, float v1, float t)
{
	return (1f -t) * v0 + t * v1;
}

float distanceSquared(float x1, float y1, float x2, float y2) {
	float deltaX = x2 - x1;
	float deltaY = y2 - y1;
	return deltaX * deltaX + deltaY * deltaY;
}
