
#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

#include <time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <lua.hpp>

#include "LTexture.h"
#include "LButton.h"
#include "LTimer.h"
#include "LGlobals.h"

#include "RectEntity.h"
#include "CircleEntity.h"
#include "Collision.h"
#include "Ant.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Ant spawning and destruction
void allocateAnts(int howmany);
void deleteAllAnts();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

// Text globals 
TTF_Font* gFont = NULL;
TTF_Font* gSmallFont = NULL;


LTexture* gAverageFPSText = NULL;
LTexture* gCurrentFPSText = NULL;
LTexture* gAntsText = NULL;
LTexture* gFrameTimeText = NULL;

LTexture* gPlayerTexture;
LTexture* gAntTexture;
LTexture* gSelectedTexture;
LTexture* gWallTexture;

CircleEntity* gDotCirclePlayer;
SDL_Rect wall = { 230,100,30,200 };

//Particles

SDL_Color textColor = { 0xFF, 0xFF ,0xFF ,0xFF };
SDL_Color wallColor = textColor;
SDL_Color backGroundColor = { 128,128,128, 0xFF };
SDL_Color dragRectColor = { 192, 192, 192, 64 };
//Event handler
SDL_Event e;

LTimer fpsTimer;
LTimer capTimer;

std::stringstream averageFpsText;
std::stringstream antsText;
std::stringstream frameTimeText;

int CREATE_THISMANY_ANTS;
const int AMOUNT_ANT_DEFAULT = 100;
int COUNT_ANTS = 0;

std::vector<Ant*> ants;

bool checkLua(lua_State* L, int r) {
	if (r != LUA_OK) {
		std::string errormsg = lua_tostring(L, -1);
		std::cout << "LUA_ERROR: " << errormsg << std::endl;
		lua_pop(L, -1);
		return false;
	}
	return true;
}
bool init()
{

	srand(666);

	//Initialization flag
	bool success = true;

	// use lua to load the settings file
	lua_State* L = luaL_newstate();

	if (checkLua(L, luaL_dofile(L, "data/settings.lua"))) {
<<<<<<< HEAD
		lua_getglobal(L, "antspawns");
		CREATE_THISMANY_ANTS = lua_tonumber(L, -1);
		if (CREATE_THISMANY_ANTS <= 0) {
			CREATE_THISMANY_ANTS = AMOUNT_ANT_DEFAULT;
=======
		if (checkLua(L, lua_getglobal(L, "antspawns"))) {
			if (checkLua(L, lua_isnumber(L, -1))) {
				CREATE_THISMANY_ANTS = lua_tonumber(L, -1);
			}
			else {
				CREATE_THISMANY_ANTS = 1;
			}
>>>>>>> 63bc13b81c7de9ea0810c3d28b2d2b499b45f5c2
		}
	}
	else {
		CREATE_THISMANY_ANTS = AMOUNT_ANT_DEFAULT;
	}

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

	gFont = TTF_OpenFont("data/font/LiberationMono-Regular.ttf", 16);
	gSmallFont = TTF_OpenFont("data/font/LiberationMono-Regular.ttf", 12);
	if (gFont == NULL || gSmallFont == NULL)
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

		gAverageFPSText = new LTexture(gRenderer, gFont);
		gCurrentFPSText = new LTexture(gRenderer, gFont);
		gAntsText = new LTexture(gRenderer, gFont);
		gFrameTimeText = new LTexture(gRenderer, gSmallFont);
		gPlayerTexture = new LTexture(gRenderer);
		gAntTexture = new LTexture(gRenderer);
		gSelectedTexture = new LTexture(gRenderer);
		gWallTexture = new LTexture(gRenderer);

	}
	
	return success;
}

bool loadMedia()
{
	gPlayerTexture->loadFromFile("data/pics/agent.png");
	gAntTexture->loadFromFile("data/pics/ant.png");
	gSelectedTexture->loadFromFile("data/pics/selected.png");
	gWallTexture->loadFromFile("data/pics/wall.png");


	gDotCirclePlayer = new CircleEntity(100, 100, gPlayerTexture, gSelectedTexture);
		
	antsText.str("");
	antsText << "Ants alive: " << COUNT_ANTS;
	gAntsText->loadFromRenderedText(antsText.str().c_str(), textColor);

	return true;
}

void SDL_SetRenderDrawColor(SDL_Renderer* r, SDL_Color& c) {
	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
}

int start()
{	
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
		return false;
	}
	
	if (!loadMedia()) {
		printf("Failed to load media!\n");
		return false;
	}


	//place some randomly placed boxes on the screen

	//Main loop flag
	bool quit = false;

	//left mouse button flags
	bool dragging = false;
	bool selectionDone = false;
	int mouseDragbeginX = 0;
	int mouseDragbeginY = 0;
	int mouseDragEndX = 0;
	int mouseDragEndY = 0;
	SDL_Rect dragRect;
			
	//right mouse button 
	bool directionGiven = false;
	int directonX = 0, directionY = 0;

	bool redrawAnts = false;
	int countedFrames = 0;
	fpsTimer.start();
	SDL_SetRenderDrawColor(gRenderer, backGroundColor);
	//While application is running
	while (!quit)
	{	
				
				

		// --- INPUT AND UPDATE  ---  

		//fps calculation and display
		float avgFPS = countedFrames++ / (fpsTimer.getTicks() / 1000.f);
		double frameTime = capTimer.getTicks() / 1000.f;
				
		if (avgFPS > 20000)avgFPS = 0;
		averageFpsText.str("");
		averageFpsText << "Average FPS: " << avgFPS;
		gAverageFPSText->loadFromRenderedText(averageFpsText.str().c_str(), textColor);

		averageFpsText.str("");
		averageFpsText << "Current FPS: " << (unsigned int)(1.f/frameTime);
		gCurrentFPSText->loadFromRenderedText(averageFpsText.str().c_str(), textColor);

		//frametime display
		frameTimeText.str("");
		frameTimeText << "last frametime: " << frameTime << " s";
		gFrameTimeText->loadFromRenderedText(frameTimeText.str().c_str(), textColor);
		capTimer.start();

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
					allocateAnts(CREATE_THISMANY_ANTS);
					redrawAnts = true;
				default:
					break;
				}
			}

			//Left mouse button for selection
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
				if (!dragging) {
					SDL_GetMouseState(&mouseDragbeginX, &mouseDragbeginY);
					SDL_GetMouseState(&mouseDragEndX, &mouseDragEndY);
					dragging = true;
				}
			}
			else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
				dragging = false;
				selectionDone = true;

			}

			//Right mouse button for move orders
			else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) {
				directionGiven = true;
				SDL_GetMouseState(&directonX, &directionY);
			}

			// Mouse motion in general
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
			gDotCirclePlayer->handleEvent(e);
		}

		//SELECTION LOGIC
		if (selectionDone) for (auto a : ants)a->mSelected = false;

		std::vector<Ant*>::iterator iter = ants.begin();

		while (iter != ants.end()) {
			Ant* a = (*iter);
			//a->updateRandomly();
			//a->move(frameTime, wall, gDotCirclePlayer->getCollider(), ants);

			if (selectionDone && checkCollision(a->getCollider(), dragRect)) a->mSelected = true;
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

		if (selectionDone) {
			selectionDone = false;
			dragRect.x = 0;
			dragRect.y = 0;
			dragRect.h = 0;
			dragRect.w = 0;
		}

		//MOVE ORDER LOGIC
		if (directionGiven) {
			for (auto a : ants) {
				if (a->mSelected) {
					a->setDestination(Vector2(directonX, directionY));
				}
			}
			directionGiven = false;
		}
				
				
		// UPDATE LOGIC
		for (auto a : ants) {
			a->proceedToDestination(frameTime, wall, gDotCirclePlayer->getCollider(), ants);
		}

		// --- RENDERING  ---  
				
		SDL_SetRenderDrawColor(gRenderer, backGroundColor);
		SDL_RenderClear(gRenderer);

		SDL_SetRenderDrawColor(gRenderer, wallColor);
		SDL_RenderFillRect(gRenderer, &wall);
				

		gDotCirclePlayer->move(frameTime, wall);
		gDotCirclePlayer->render();


		for (Ant* a : ants)a->render();

		if (dragging) {
			SDL_SetRenderDrawColor(gRenderer, dragRectColor);
			SDL_RenderFillRect(gRenderer, &dragRect);
			SDL_SetRenderDrawColor(gRenderer, backGroundColor);
		}
						//draw FPS over everything
		gAverageFPSText->render(10, 10);
		gCurrentFPSText->render(10, 40);
		gAntsText->render(10, 70);
		gFrameTimeText->render(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 10);
		SDL_RenderPresent(gRenderer);
	}	

	//Free resources and close SDL
	close();
	return 0;
}


void allocateAnts(int howmany) {
	int c = 0;
	while (c < howmany) {
		float x = rand() % (SCREEN_WIDTH - CircleEntity::DOT_WIDTH) + CircleEntity::DOT_WIDTH;
		float y = rand() % (SCREEN_HEIGHT - CircleEntity::DOT_HEIGHT) + CircleEntity::DOT_HEIGHT;

		Ant* ant = new Ant(x, y, gAntTexture,gSelectedTexture);

		if (checkCollision(ant->getCollider(), wall) || checkCollision(ant, ants)) {
			delete ant;
		}
		else {
			ants.push_back(ant);
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

void close()
{
	delete gAverageFPSText;
	delete gCurrentFPSText;
	delete gAntsText;
	delete gPlayerTexture;
	delete gAntTexture;
	delete gSelectedTexture;
	delete gWallTexture;
	delete gDotCirclePlayer;

	deleteAllAnts();

	TTF_CloseFont(gFont);

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


