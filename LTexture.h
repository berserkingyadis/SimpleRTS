#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

class LTexture
{
private:
	SDL_Renderer* mRenderer;
	TTF_Font* mFont;
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
public:
	LTexture( SDL_Renderer* renderer = NULL, TTF_Font* font = NULL);
	~LTexture();

	bool loadFromFile(std::string path);
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	void free();

	void setColor(uint8_t red, uint8_t green, uint8_t blue);

	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(uint8_t alpha);
	void render(int x = 0, int y = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();
};

#endif