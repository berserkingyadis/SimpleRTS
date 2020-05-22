#pragma once
#ifndef COLLISION_H
#define COLLISION_H

#include "SDL.h"

#include "CircleEntity.h"
#include "Ant.h"
#include "LGlobals.h"
#include "math.h"

#include <vector>

bool checkCollision(SDL_Rect& a, SDL_Rect& b);
bool checkCollision(Circle& a, Circle& b);
bool checkCollision(Circle& a, SDL_Rect& b);

bool checkCollision(Ant* dot, const std::vector<Ant*> &all);
#endif