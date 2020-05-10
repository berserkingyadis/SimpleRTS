//Using SDL, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>
#include <cmath>

#include "LTexture.h"
#include "LButton.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TEXTURES_SIZE = 1;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Texture* loadTexture(std::string path);


LTexture* gTextures[TEXTURES_SIZE];
LTexture* gTexMila;
LTexture* gSprites;
SDL_Rect gDrawingSpriteClips[4];

//Walking animation
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
LTexture* gWalkingAnim;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

LTexture* gText = NULL;

const int TOTAL_BUTTONS = 4;
SDL_Rect gButtonClips[TOTAL_BUTTONS];

LButton gButtons[TOTAL_BUTTONS];



bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{

			//Init PNG loading powah
			int imgFlags = IMG_INIT_PNG;

			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				success = false;
			}
			else
			{
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}

				gFont = TTF_OpenFont("font/lazy.ttf", 50);
				if (gFont == NULL)
				{
					printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

				if (gRenderer == NULL)
				{
					printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
					success = false;
				}
				else
				{


					//Initialize renderer color
					for (size_t i = 0; i < TEXTURES_SIZE; i++)
					{
						gTextures[i] = new LTexture(gRenderer);
					}
					gTexMila = new LTexture(gRenderer);
					gSprites = new LTexture(gRenderer);
					gWalkingAnim = new LTexture(gRenderer);
					gText = new LTexture(gRenderer, gFont);
					int size = sizeof(*gText);
					printf("texture size is " + size);
				}

			}
		}
	}

	return success;
}
bool loadMedia()
{
	if (!gTextures[0]->loadFromFile("pics/back.png")) {
		printf("Failed to load image 0.png");
		return false;
	}
	if (!gTexMila->loadFromFile("pics/mila.png")) {
		printf("Failed to load mila :(");
		return false;
	}
	else {
		gTexMila->setBlendMode(SDL_BLENDMODE_BLEND);
	}
	if (!gSprites->loadFromFile("pics/sprites.png")) {
		printf("Failed to load sprites :(");
		return false;
	}
	if (!gWalkingAnim->loadFromFile("pics/walk.png")) {
		printf("Failed to load walking animation :(");
		return false;
	}
	//Set top left sprite
	gDrawingSpriteClips[0].x = 0;
	gDrawingSpriteClips[0].y = 0;
	gDrawingSpriteClips[0].w = 100;
	gDrawingSpriteClips[0].h = 100;

	//Set top right sprite
	gDrawingSpriteClips[1].x = 100;
	gDrawingSpriteClips[1].y = 0;
	gDrawingSpriteClips[1].w = 100;
	gDrawingSpriteClips[1].h = 100;

	//Set bottom left sprite
	gDrawingSpriteClips[2].x = 0;
	gDrawingSpriteClips[2].y = 100;
	gDrawingSpriteClips[2].w = 100;
	gDrawingSpriteClips[2].h = 100;

	//Set bottom right sprite
	gDrawingSpriteClips[3].x = 100;
	gDrawingSpriteClips[3].y = 100;
	gDrawingSpriteClips[3].w = 100;
	gDrawingSpriteClips[3].h = 100;

	//Set sprite clips for walking animation
	gSpriteClips[0].x = 0;
	gSpriteClips[0].y = 0;
	gSpriteClips[0].w = 64;
	gSpriteClips[0].h = 205;

	gSpriteClips[1].x = 64;
	gSpriteClips[1].y = 0;
	gSpriteClips[1].w = 64;
	gSpriteClips[1].h = 205;

	gSpriteClips[2].x = 128;
	gSpriteClips[2].y = 0;
	gSpriteClips[2].w = 64;
	gSpriteClips[2].h = 205;

	gSpriteClips[3].x = 196;
	gSpriteClips[3].y = 0;
	gSpriteClips[3].w = 64;
	gSpriteClips[3].h = 205;

	SDL_Color textColor = { 0,0,0 };
	if (!gText->loadFromRenderedText("MILA IS CUTE ! :3", textColor))
	{
		printf("Failed to render text texture!\n");
		return false;
	}
	
	return true;
}

void close()
{


	//Deallocate surfaces
	for (size_t i = 0; i < TEXTURES_SIZE; i++)
	{
		delete gTextures[i];
		gTextures[i] = NULL;
	}

	delete gTexMila;
	gTexMila = NULL;

	delete gSprites;
	gSprites = NULL;

	delete gWalkingAnim;
	gWalkingAnim = NULL;

	delete gText;
	gText = NULL;

	TTF_CloseFont(gFont);
	gFont = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}



int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Color modulation components
			uint8_t r = 255;
			uint8_t g = 255;
			uint8_t b = 255;

			//Alpha modulation component
			uint8_t a = 255;

			//Mila position
			int milaX = 0;
			int milaY = 0;

			//current animation frame
			int frame = 0;

			//angle of rotaiton
			double degrees = 0;

			//flip type
			SDL_RendererFlip fliptype = SDL_FLIP_NONE;

			//While application is running
			while (!quit)
			{	
				SDL_RenderClear(gRenderer);
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					//User presses a key
					else if (e.type == SDL_KEYDOWN)
					{
						//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

						//Select surfaces based on key press
						switch (e.key.keysym.sym)
						{
						case SDLK_ESCAPE:
							quit = true;
							break;
						case SDLK_UP:
							milaY = (milaY - 5) % (SCREEN_HEIGHT - gTexMila->getHeight());
							break;
						case SDLK_DOWN:
							milaY = (milaY + 5) % (SCREEN_HEIGHT - gTexMila->getHeight());
							break;
						case SDLK_LEFT:
							if (fliptype == SDL_FLIP_NONE)fliptype = SDL_FLIP_HORIZONTAL;
							milaX = (milaX - 5) % (SCREEN_WIDTH - gTexMila->getWidth());
							break;
						case SDLK_RIGHT:
							if (fliptype == SDL_FLIP_HORIZONTAL)fliptype = SDL_FLIP_NONE;
							milaX = (milaX + 5) % (SCREEN_WIDTH - gTexMila->getWidth());
							break;
						case SDLK_q:
							r += 32;
							break;
						case SDLK_w:
							r -= 32;
							break;
						case SDLK_a:
							g += 32;
							break;
						case SDLK_s:
							g -= 32;
							break;
						case SDLK_y:
							b += 32;
							break;
						case SDLK_x:
							b -= 32;
							break;
						case SDLK_e:
							a += 32;
							break;
						case SDLK_r:
							a -= 32;
							break;
						case SDLK_d:
							degrees += 15;
							break;
						case SDLK_f:
							degrees -= 15;
							break;
						default:
							break;
						}

						gTexMila->setColor(r, g, b);
					}
				}
				
				gTextures[0]->render();
				gTexMila->setAlpha(a);
				gTexMila->render(milaX,milaY,NULL,degrees, NULL, fliptype);

				gSprites->render(SCREEN_WIDTH - gDrawingSpriteClips[0].w, SCREEN_HEIGHT - gDrawingSpriteClips[0].h, &gDrawingSpriteClips[0]);

				SDL_Rect* currentClip = &gSpriteClips[frame / 4];
				gWalkingAnim->render(SCREEN_WIDTH - currentClip->w, 0, currentClip);

				gText->render(200, 200);
				SDL_RenderPresent(gRenderer);

				SDL_Delay(10);
				
				++frame;
				if (frame / 4 >= WALKING_ANIMATION_FRAMES) {
					frame = 0;
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

