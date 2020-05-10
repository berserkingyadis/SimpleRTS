//Using SDL, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>

#include <stdio.h>
#include <string>

#include "LTexture.h"

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


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;


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
				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

				if (gRenderer == NULL)
				{
					printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
					success = false;
				}
				else
				{


					//Initialize renderer color
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					for (size_t i = 0; i < TEXTURES_SIZE; i++)
					{
						gTextures[i] = new LTexture(gRenderer);
					}
					gTexMila = new LTexture(gRenderer);
				}

			}
		}
	}

	return success;
}
bool loadMedia()
{
	if (!gTextures[0]->loadFromFile("0.png")) {
		printf("Failed to load image 0.png");
		return false;
	}
	if (!gTexMila->loadFromFile("mila.png")) {
		printf("Failed to load mila :(");
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

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
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

			//While application is running
			while (!quit)
			{

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
						case SDLK_UP:
						case SDLK_DOWN:
						case SDLK_LEFT:
						case SDLK_RIGHT:
						case SDLK_r:
							quit = true;
							break;
						default:
							break;
						}
					}
				}
				SDL_RenderClear(gRenderer);
				gTextures[0]->render();
				gTexMila->render(0, 0+(SCREEN_HEIGHT - gTexMila->getHeight()));

				SDL_RenderPresent(gRenderer);

				SDL_Delay(10);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}