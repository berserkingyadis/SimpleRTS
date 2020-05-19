#pragma once
#ifndef COLLISION_H
#define COLLISION_H

#include "SDL.h"
#include "LGlobals.h"

bool checkCollision(SDL_Rect& a, SDL_Rect& b);
bool checkCollision(Circle& a, Circle& b);
bool checkCollision(Circle& a, SDL_Rect& b);
float distanceSquared(float x1, float y1, float x2, float y2);
#endif