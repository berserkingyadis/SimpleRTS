#ifndef LBUTTON_H
#define LBUTTON_H

#include "SDL.h"
#include "SDL_image.h"


const int BUTTON_WIDTH = 100;
const int BUTTON_HEIGHT = 50;


enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

class LButton
{
public:
	LButton(SDL_Renderer* renderer = NULL);
	void setPosition(int x, int y);
	void handleEvent(SDL_Event* e);

	void render();
private:
	SDL_Renderer* mRenderer;
	SDL_Point mPosition;
	LButtonSprite mCurrentSprite;
};

#endif
