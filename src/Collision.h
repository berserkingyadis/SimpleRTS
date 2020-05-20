#pragma once
#ifndef COLLISION_H
#define COLLISION_H

#include "SDL.h"
#include "LGlobals.h"
#include "math.h"

bool checkCollision(SDL_Rect& a, SDL_Rect& b);
bool checkCollision(Circle& a, Circle& b);
bool checkCollision(Circle& a, SDL_Rect& b);

#endif