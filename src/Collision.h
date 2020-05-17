#pragma once
#ifndef COLLISION_H
#define COLLISION_H

#include "SDL.h"
#include "LGlobals.h"

bool checkCollision(SDL_Rect& a, SDL_Rect& b);
bool checkCollision(Circle& a, Circle& b);
bool checkCollision(Circle& a, SDL_Rect& b);
double distanceSquared(int x1, int y1, int x2, int y2);

#endif