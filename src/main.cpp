
#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_thread.h>

#include "LTexture.h"
#include "LButton.h"
#include "LTimer.h"
#include "LGlobals.h"

#include "DotRect.h"
#include "DotCircle.h"


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

// Text globals 
TTF_Font* gSmallFont = NULL;

LTexture* gFPSText = NULL;

LTexture* gDotTexture;
DotCircle* gDotCirclePlayer;
DotCircle* gDotCircleStatic;
SDL_Rect wall = { 230,100,30,200 };

//Particles
LTexture* gRedTexture;
LTexture* gGreenTexture;
LTexture* gBlueTexture;
LTexture* gShimmerTexture;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{	
		//init sdl_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			success = false;
		}
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

				gSmallFont = TTF_OpenFont("data/font/LiberationMono-Regular.ttf", 18);
				if (gSmallFont == NULL)
				{
					printf("Failed to load  font! SDL_ttf Error: %s\n", TTF_GetError());
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
					gFPSText = new LTexture(gRenderer, gSmallFont);
					gDotTexture = new LTexture(gRenderer);
					gRedTexture = new LTexture(gRenderer);
					gGreenTexture = new LTexture(gRenderer);
					gBlueTexture = new LTexture(gRenderer);
					gShimmerTexture = new LTexture(gRenderer);

				}
			}
		}
	}
	return success;
}
bool loadMedia()
{
	gDotTexture->loadFromFile("data/pics/agent.png");
	gRedTexture->loadFromFile("data/pics/particles/red.png");
	gGreenTexture->loadFromFile("data/pics/particles/green.png");
	gBlueTexture->loadFromFile("data/pics/particles/blue.png");
	gShimmerTexture->loadFromFile("data/pics/particles/shimmer.png");

	gRedTexture->setAlpha(192);
	gGreenTexture->setAlpha(192);
	gBlueTexture->setAlpha(192);
	gShimmerTexture->setAlpha(192);

	gDotCirclePlayer = new DotCircle(100, 100, gDotTexture);
	gDotCircleStatic = new DotCircle(200, 200, gDotTexture);

	return true;
}

void SDL_SetRenderDrawColor(SDL_Renderer* r, SDL_Color& c) {
	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
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

			//fps timer
			SDL_Color textColor = {0, 0, 0, 0xFF};
			SDL_Color wallColor = textColor;
			SDL_Color backGroundColor = { 50,50,50, 0xFF };

			LTimer fpsTimer;
			LTimer capTimer;

			std::stringstream fpsText;

			int countedFrames = 0;
			fpsTimer.start();
			SDL_SetRenderDrawColor(gRenderer, backGroundColor);
			//While application is running
			while (!quit)
			{	

				capTimer.start();
				
				bool renderText = false;

				SDL_RenderClear(gRenderer);

				//fps calculation and display
				float avgFPS = countedFrames++ / (fpsTimer.getTicks() / 1000.f);
				if (avgFPS > 20000)avgFPS = 0;
				fpsText.str("");
				fpsText << "Average FPS: " << avgFPS;

				gFPSText->loadFromRenderedText(fpsText.str().c_str(), textColor);

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
						//Select surfaces based on key press
						switch (e.key.keysym.sym)
						{
						case SDLK_ESCAPE:
							quit = true;
							break;
						default:
							break;
						}
					}
					gDotCirclePlayer->handleEvent(e);
				}



				SDL_SetRenderDrawColor(gRenderer, wallColor);
				SDL_RenderFillRect(gRenderer, &wall);
				SDL_SetRenderDrawColor(gRenderer, backGroundColor);

				gDotCirclePlayer->move(wall, gDotCircleStatic->getCollider());
				gDotCirclePlayer->render();
				gDotCircleStatic->render();

				//draw FPS over everything
				gFPSText->render(10, 10);
				SDL_RenderPresent(gRenderer);


				int frameTicks = capTimer.getTicks();
				if (frameTicks < SCREEN_TICKS_PER_FRAME) {
					//SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
					//SDL_Delay(19);
				}
			}
		}
	}

	//Free resources and close SDL
	close();
	SDL_StopTextInput();
	return 0;
}


void close()
{
	delete gFPSText;
	delete gRedTexture;
	delete gGreenTexture;
	delete gBlueTexture;
	delete gShimmerTexture;

	TTF_CloseFont(gSmallFont);

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

