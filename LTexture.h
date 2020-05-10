#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>

#include <string>

class LTexture
{
private:
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
public:
	LTexture( SDL_Renderer* renderer);
	~LTexture();

	bool loadFromFile(std::string path);
	
	void free();
	void render(int x = 0, int y = 0, SDL_Rect* clip = NULL);

	int getWidth();
	int getHeight();
};



#endif