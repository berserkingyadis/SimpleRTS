
#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

#include <time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "LTexture.h"
#include "LButton.h"
#include "LTimer.h"
#include "LGlobals.h"

#include "DotRect.h"
#include "DotCircle.h"
#include "Collision.h"


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
LTexture* gAntsText = NULL;

LTexture* gPlayerTexture;
LTexture* gAntTexture;

DotCircle* gDotCirclePlayer;
DotCircle* gDotCircleStatic;
SDL_Rect wall = { 230,100,30,200 };

//Particles
LTexture* gRedTexture;
LTexture* gGreenTexture;
LTexture* gBlueTexture;
LTexture* gShimmerTexture;

SDL_Color textColor = { 0xFF, 0xFF ,0xFF ,0xFF };
SDL_Color wallColor = textColor;
SDL_Color backGroundColor = { 50,50,50, 0xFF };
SDL_Color dragRectColor = { 192, 192, 192, 64 };
//Event handler
SDL_Event e;


LTimer fpsTimer;
LTimer capTimer;

std::stringstream fpsText;
std::stringstream antsText;

const int CREATE_THISMANY_ANTS = 30000;
int COUNT_ANTS = 0;

std::vector<DotCircleAnt*> ants;

bool init()
{

	srand(time(NULL));

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
		
	//Create window
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
		

	//Init PNG loading powah
	int imgFlags = IMG_INIT_PNG;

	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		success = false;
	}
			
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
		SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

		gFPSText = new LTexture(gRenderer, gSmallFont);
		gAntsText = new LTexture(gRenderer, gSmallFont);
		gPlayerTexture = new LTexture(gRenderer);
		gAntTexture = new LTexture(gRenderer);
		gRedTexture = new LTexture(gRenderer);
		gGreenTexture = new LTexture(gRenderer);
		gBlueTexture = new LTexture(gRenderer);
		gShimmerTexture = new LTexture(gRenderer);

	}
	
	return success;
}

void allocateAnts() {
	int c = 0;
	while (c < CREATE_THISMANY_ANTS) {
		int x = rand() % (SCREEN_WIDTH - DotCircle::DOT_WIDTH) + DotCircle::DOT_WIDTH;
		int y = rand() % (SCREEN_HEIGHT - DotCircle::DOT_HEIGHT) + DotCircle::DOT_HEIGHT;

		DotCircleAnt* a = new DotCircleAnt(x, y, gAntTexture);

		if (checkCollision(a->getCollider(), wall) || checkCollision(a->getCollider(), gDotCircleStatic->getCollider())) {
			delete a;
		}
		else {
			ants.push_back(a);
			c++;
		}
	}
	COUNT_ANTS += CREATE_THISMANY_ANTS;
}

void deleteAllAnts() {
	for (auto p : ants) delete p;
	ants.clear();
	COUNT_ANTS = 0;
}
bool loadMedia()
{
	gPlayerTexture->loadFromFile("data/pics/agent.png");
	gAntTexture->loadFromFile("data/pics/ant.png");
	gRedTexture->loadFromFile("data/pics/particles/red.png");
	gGreenTexture->loadFromFile("data/pics/particles/green.png");
	gBlueTexture->loadFromFile("data/pics/particles/blue.png");
	gShimmerTexture->loadFromFile("data/pics/particles/shimmer.png");

	gRedTexture->setAlpha(192);
	gGreenTexture->setAlpha(192);
	gBlueTexture->setAlpha(192);
	gShimmerTexture->setAlpha(192);

	gDotCirclePlayer = new DotCircle(100, 100, gPlayerTexture);
	gDotCircleStatic = new DotCircle(200, 200, gPlayerTexture);

	
	//place COUNT_ANTS random ant dots
	allocateAnts();
		

		//if the created ant collides with the wall, do not add it.
		
	antsText.str("");
	antsText << "Ants alive: " << COUNT_ANTS;
	gAntsText->loadFromRenderedText(antsText.str().c_str(), textColor);

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

			bool dragging = false;
			bool killSwitch = false;
			int mouseDragbeginX = 0;
			int mouseDragbeginY = 0;
			int mouseDragEndX = 0;
			int mouseDragEndY = 0;
			SDL_Rect dragRect;
			

			bool redrawAnts = false;
			int countedFrames = 0;
			fpsTimer.start();
			SDL_SetRenderDrawColor(gRenderer, backGroundColor);
			//While application is running
			while (!quit)
			{	
				capTimer.start();
				

				// --- INPUT AND UPDATE  ---  

				//fps calculation and display
				float avgFPS = countedFrames++ / (fpsTimer.getTicks() / 1000.f);
				if (avgFPS > 20000)avgFPS = 0;
				fpsText.str("");
				fpsText << "Average FPS: " << avgFPS;
				gFPSText->loadFromRenderedText(fpsText.str().c_str(), textColor);

				if (redrawAnts) {
					antsText.str("");
					antsText << "Ants alive: " << COUNT_ANTS;
					gAntsText->loadFromRenderedText(antsText.str().c_str(), textColor);
				}
				redrawAnts = false;

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
						case SDLK_e:
							deleteAllAnts();
							redrawAnts = true;
							break;
						case SDLK_r:
							allocateAnts();
							redrawAnts = true;
						default:
							break;
						}
					}
					else if (e.type == SDL_MOUSEBUTTONDOWN) {
						if (!dragging) {
							SDL_GetMouseState(&mouseDragbeginX, &mouseDragbeginY);
							SDL_GetMouseState(&mouseDragEndX, &mouseDragEndY);
							dragging = true;
						}
					}
					else if (e.type == SDL_MOUSEMOTION) {
						if (dragging) {
							SDL_GetMouseState(&mouseDragEndX, &mouseDragEndY);
							dragRect.x = MIN(mouseDragbeginX, mouseDragEndX);
							dragRect.y = MIN(mouseDragbeginY, mouseDragEndY);
							int w = mouseDragbeginX - mouseDragEndX;
							int h = mouseDragbeginY - mouseDragEndY;
							dragRect.w = ABS(w);
							dragRect.h = ABS(h);
						}
					}
					else if (e.type == SDL_MOUSEBUTTONUP) {
						dragging = false;
						killSwitch = true;
					}
					gDotCirclePlayer->handleEvent(e);
				}


				std::vector<DotCircleAnt*>::iterator iter = ants.begin();
				
				while (iter != ants.end()) {
					DotCircleAnt* a = (*iter);
					a->update();
					a->move(wall, gDotCircleStatic->getCollider(), gDotCirclePlayer->getCollider());

					if (killSwitch && checkCollision(a->getCollider(), dragRect)) a->mDead = true;
					if (a->mDead) {
						iter = ants.erase(iter);
						delete a;
						COUNT_ANTS--;
						redrawAnts = true;
					}
					else {
						++iter;
					}
				}
				
				if (killSwitch)killSwitch = false;

				// --- RENDERING  ---  
				
				
				SDL_RenderClear(gRenderer);

				SDL_SetRenderDrawColor(gRenderer, wallColor);
				SDL_RenderFillRect(gRenderer, &wall);
				SDL_SetRenderDrawColor(gRenderer, backGroundColor);

				gDotCirclePlayer->move(wall, gDotCircleStatic->getCollider());
				gDotCirclePlayer->render();
				gDotCircleStatic->render();


				for (DotCircleAnt* a : ants)a->render();

				if (dragging) {
					SDL_SetRenderDrawColor(gRenderer, dragRectColor);
					SDL_RenderFillRect(gRenderer, &dragRect);
					SDL_SetRenderDrawColor(gRenderer, backGroundColor);
				}
								//draw FPS over everything
				gFPSText->render(10, 10);
				gAntsText->render(10, 40);

				SDL_RenderPresent(gRenderer);
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
	delete gAntsText;
	delete gRedTexture;
	delete gGreenTexture;
	delete gBlueTexture;
	delete gShimmerTexture;
	delete gPlayerTexture;
	delete gAntTexture;

	deleteAllAnts();

	TTF_CloseFont(gSmallFont);

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

