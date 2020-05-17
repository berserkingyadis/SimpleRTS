//Using SDL, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_thread.h>

#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>


#include "LTexture.h"
#include "LButton.h"
#include "LTimer.h"
#include "LGlobals.h"

#include "DotRect.h"
#include "DotCircle.h"




const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();


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

// Text globals 
TTF_Font* gSmallFont = NULL;

LTexture* gInputText = NULL;
LTexture* gFPSText = NULL;

// Button globals
const int TOTAL_BUTTONS = 4;
SDL_Rect gButtonClips[TOTAL_BUTTONS];

LButton* gButtons[TOTAL_BUTTONS];
LTexture* gTexButtons;

// music globals
Mix_Music *gMusic = NULL;

// button 
LTexture* gDotTexture;
DotRect* gDot;

//button 2
DotCircle* gDotCirclePlayer;
DotCircle* gDotCircleStatic;
SDL_Rect wall = { 230,100,30,200 };



bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_AUDIO) < 0)
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
					gTexMila = new LTexture(gRenderer);
					gSprites = new LTexture(gRenderer);
					gWalkingAnim = new LTexture(gRenderer);
					gInputText = new LTexture(gRenderer, gSmallFont);
					gFPSText = new LTexture(gRenderer, gSmallFont);
					gTexButtons = new LTexture(gRenderer);
					gDotTexture = new LTexture(gRenderer);

					//load button texture early
					if (!gTexButtons->loadFromFile("data/button/buttons.png")) {
						printf("Failed to load buttons :(");
						success = false;
					}
					//init button clips
					for (int i = 0; i < TOTAL_BUTTONS; i++) {
						gButtonClips[i].x = 0;
						gButtonClips[i].y = i * 200;
						gButtonClips[i].w = BUTTON_WIDTH;
						gButtonClips[i].h = BUTTON_HEIGHT;

						gButtons[i] = new LButton(gRenderer, gButtonClips, gTexButtons);
					}
					gButtons[0]->setPosition(0, 0);
					gButtons[1]->setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
					gButtons[2]->setPosition(0, SCREEN_HEIGHT-BUTTON_HEIGHT);
					gButtons[3]->setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
				}
			}
		}
	}
	return success;
}
bool loadMedia()
{
	if (!gTexMila->loadFromFile("data/pics/mila.png")) {
		printf("Failed to load mila :(");
		return false;
	}
	if (!gSprites->loadFromFile("data/pics/sprites.png")) {
		printf("Failed to load sprites :(");
		return false;
	}
	if (!gWalkingAnim->loadFromFile("data/pics/walk.png")) {
		printf("Failed to load walking animation :(");
		return false;
	}

	if (!gDotTexture->loadFromFile("data/pics/dot.png")) {
		printf("Failed to load dot :(");
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

	gMusic = Mix_LoadMUS("data/sound/beat.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	gDot = new DotRect(gDotTexture);

	gDotCirclePlayer = new DotCircle(100, 100, gDotTexture);
	gDotCircleStatic = new DotCircle(200, 200, gDotTexture);

	return true;
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

			//joystick 
			int xDir = 0;
			int yDir = 0;

			//my timer
			LTimer timer;
			std::stringstream timeText;

			//fps timer
			SDL_Color textColor = {0, 0, 0};

			LTimer fpsTimer;
			LTimer capTimer;

			std::stringstream fpsText;

			//text input stuff
			std::string inputText = "Some text";
			gInputText->loadFromRenderedText(inputText.c_str(),textColor);


			int countedFrames = 0;
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			fpsTimer.start();
			//While application is running
			while (!quit)
			{	

				capTimer.start();
				
				bool renderText = false;

				SDL_RenderClear(gRenderer);

				//fps calculation and display
				float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
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
						case SDLK_RETURN:
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
						case SDLK_t:
							if (timer.isStarted()) {
								timer.stop();
							}
							else {
								timer.start();
							}
							break;
						case SDLK_z:
							if (timer.isPaused()) {
								timer.unpause();
							}
							else {
								timer.pause();
							}
							break;
						case SDLK_u:
							timeText.str("");
							timeText << "Seconds since start of timer " << (timer.getTicks() / 1000.f) << "\n";
							printf(timeText.str().c_str());
							break;
						default:
							break;
						}
						gTexMila->setColor(r, g, b);
					}
					//Handle button events
					for (size_t i = 0; i < TOTAL_BUTTONS; i++) {
						gButtons[i]->handleEvent(e);
					}
					gDotCirclePlayer->handleEvent(e);
				}

				//check ESC key state to exit
				const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);
				if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
					quit = true;
				}
				//use C to start/pause the music
				if (currentKeyStates[SDL_SCANCODE_C]) {
					if (Mix_PlayingMusic() == 0) {
						Mix_PlayMusic(gMusic, -1);
					}
					else {
						if (Mix_PausedMusic() == 1) {
							Mix_ResumeMusic();
						}
						else {
							Mix_PauseMusic();
						}
					}
				}
				if (currentKeyStates[SDL_SCANCODE_V]) {
					Mix_HaltMusic();
				}

				gTexMila->setAlpha(a);
				gTexMila->render(0, SCREEN_HEIGHT-gTexMila->getHeight(), NULL, degrees, NULL);

				gSprites->render(SCREEN_WIDTH - gDrawingSpriteClips[0].w, SCREEN_HEIGHT - gDrawingSpriteClips[0].h, &gDrawingSpriteClips[0]);

				SDL_Rect* currentClip = &gSpriteClips[frame / 4];
				
				gWalkingAnim->render(SCREEN_WIDTH - currentClip->w, 0, currentClip);



				SDL_SetRenderDrawColor(gRenderer, 50, 50,50, 0xFF);
				SDL_RenderFillRect(gRenderer, &wall);
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				gButtons[3]->render();
				gDotCirclePlayer->move(wall, gDotCircleStatic->getCollider());
				gDotCirclePlayer->render();
				gDotCircleStatic->render();

				gInputText->render(300, 150);
				//draw FPS over everything
				gFPSText->render(10, 10);
				SDL_RenderPresent(gRenderer);

				++frame;
				if (frame / 4 >= WALKING_ANIMATION_FRAMES) {
					frame = 0;
				}
				++countedFrames;

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
	for (size_t i = 0; i < TOTAL_BUTTONS; i++) {
		delete gButtons[i];
		gButtons[i] = NULL;
	}

	delete gTexMila;
	gTexMila = NULL;

	delete gSprites;
	gSprites = NULL;

	delete gWalkingAnim;
	gWalkingAnim = NULL;

	delete gInputText;
	gInputText = NULL;

	delete gFPSText;
	gFPSText = NULL;

	delete gTexButtons;
	gTexButtons = NULL;

	TTF_CloseFont(gSmallFont);
	gSmallFont = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	Mix_FreeMusic(gMusic);
	gMusic = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

