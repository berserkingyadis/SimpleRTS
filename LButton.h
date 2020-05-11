#ifndef LBUTTON_H
#define LBUTTON_H

#include "SDL.h"
#include "SDL_image.h"

#include "LTexture.h"




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
	LButton(SDL_Renderer* renderer = NULL, SDL_Rect* clip = NULL, LTexture* texture = NULL);
	void setPosition(int x, int y);
	void handleEvent(SDL_Event* e);

	void render();
private:
	SDL_Renderer* mRenderer;
	SDL_Rect* mClip;
	LTexture* mTexture;
	SDL_Point mPosition;
	int mWidth;
	int mHeight;
	LButtonSprite mCurrentSprite;
};

#endif
