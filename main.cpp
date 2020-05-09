#define VERSION "0.2.1"

#include <stdio.h>

#include "SDL.h"


//initializes SDL
bool init();

//loads media
bool loadMedia();

//frees media and shuts down sdl
void close();	

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;					//game window
SDL_Surface* gScreenSurface = NULL;			//surface in game window
SDL_Surface* gHelloWorld = NULL;			//image to load


int main(int argc, char *argv[]) {

	init();

	loadMedia();

	SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
	SDL_UpdateWindowSurface(gWindow);
	SDL_Delay(5000);
	return 0;
}

bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;

	gHelloWorld = SDL_LoadBMP("hello.bmp");
	if (gHelloWorld == NULL) {
		printf("Unable to load image %s! SDL Error: %s\n", "hello.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}