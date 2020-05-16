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

#define M_PI 3.141592653589793
#include "LTexture.h"
#include "LButton.h"
#include "LTimer.h"
#include "LGlobals.h"
#include "Dot.h"



const int TEXTURES_SIZE = 1;

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();


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

// Text globals 
TTF_Font* gBigFont = NULL;
TTF_Font* gSmallFont = NULL;

LTexture* gText = NULL;
LTexture* gFPSText = NULL;

// Button globals
const int TOTAL_BUTTONS = 4;
SDL_Rect gButtonClips[TOTAL_BUTTONS];

LButton* gButtons[TOTAL_BUTTONS];
LTexture* gTexButtons;

// gamepad globals
const int JOYSTICK_DEAD_ZONE = 5000;
SDL_Joystick* gGameController = NULL;
SDL_Haptic* gControllerHaptic = NULL;

// music globals
Mix_Music *gMusic = NULL;

// button 
LTexture* gDotTexture;
Dot* gDot;

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
		//Check for joysticks
		if (SDL_NumJoysticks() < 1) {
			printf("Warning: No joysticks have been found by SDL!\n");
		}
		else {
			//Load Joystick
			gGameController = SDL_JoystickOpen(0);
			if (gGameController == NULL)
			{
				printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
			}
			else {
				//Get controller haptic device
				gControllerHaptic = SDL_HapticOpenFromJoystick(gGameController);
				if (gControllerHaptic == NULL)
				{
					printf("Warning: Controller does not support haptics! SDL Error: %s\n", SDL_GetError());
				}
				else
				{
					//Get initialize rumble
					if (SDL_HapticRumbleInit(gControllerHaptic) < 0)
					{
						printf("Warning: Unable to initialize rumble! SDL Error: %s\n", SDL_GetError());
					}
				}
			}
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

				gBigFont = TTF_OpenFont("data/font/LiberationMono-Regular.ttf", 26);
				gSmallFont = TTF_OpenFont("data/font/LiberationMono-Regular.ttf", 18);
				if (gBigFont == NULL || gSmallFont == NULL)
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
					gText = new LTexture(gRenderer, gBigFont);
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
	if (!gTextures[0]->loadFromFile("data/pics/back.png")) {
		printf("Failed to load background");
		return false;
	}
	if (!gTexMila->loadFromFile("data/pics/mila.png")) {
		printf("Failed to load mila :(");
		return false;
	}
	else {
		gTexMila->setBlendMode(SDL_BLENDMODE_BLEND);
	}
	if (!gSprites->loadFromFile("data/pics/sprites.png")) {
		printf("Failed to load sprites :(");
		return false;
	}
	if (!gWalkingAnim->loadFromFile("data/pics/walk.png")) {
		printf("Failed to load walking animation :(");
		return false;
	}

	if (!gDotTexture->loadFromFile("data/pics/button.png")) {
		printf("Failed to load button :(");
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
	if (!gText->loadFromRenderedText("Press Enter to Reset start time :)", textColor))
	{
		printf("Failed to render text texture!\n");
		return false;
	}
	
	gMusic = Mix_LoadMUS("data/sound/beat.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	gDot = new Dot(gDotTexture, gRenderer);


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

			//flip type
			SDL_RendererFlip fliptype = SDL_FLIP_NONE;

			//joystick 
			int xDir = 0;
			int yDir = 0;

			//my timer
			LTimer timer;
			std::stringstream timeText;

			//fps timer
			SDL_Color textColor = {0xFF, 0xFF, 0xFF};

			LTimer fpsTimer;
			LTimer capTimer;

			std::stringstream fpsText;
			int countedFrames = 0;
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			fpsTimer.start();
			//While application is running
			while (!quit)
			{
				capTimer.start();
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
						//SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

						//Select surfaces based on key press
						switch (e.key.keysym.sym)
						{
						case SDLK_RETURN:
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
					else if (e.type == SDL_JOYAXISMOTION) {
						//X axis motion
						if (e.jaxis.axis == 0)
						{
							//Left of dead zone
							if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
							{
								xDir = -1;
							}
							//Right of dead zone
							else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
							{
								xDir = 1;
							}
							else
							{
								xDir = 0;
							}
						}
						//Y axis motion
						else if (e.jaxis.axis == 1)
						{
							//Below of dead zone
							if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
							{
								yDir = -1;
							}
							//Above of dead zone
							else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
							{
								yDir = 1;
							}
							else
							{
								yDir = 0;
							}
						}
					}
					//Handle button events
					for (size_t i = 0; i < TOTAL_BUTTONS; i++) {
						gButtons[i]->handleEvent(e);
					}
					gDot->handleEvent(e);
				}

				//calculate joystick angle
				double joystickAngle = atan2((double)yDir, (double)xDir) * (180.0 / M_PI);

				//Correct angle
				if (xDir == 0 && yDir == 0)
				{
					joystickAngle = 0;
				}
				//check ESC key state to exit
				const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);
				if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
					quit = true;
				}
				if (currentKeyStates[SDL_SCANCODE_B]) {
					if (SDL_HapticRumblePlay(gControllerHaptic, 1, 20000) != 0)
					{
						printf("Warning: Unable to play rumble! %s\n", SDL_GetError());
					}
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

				gTextures[0]->render();
				gTexMila->setAlpha(a);
				gTexMila->render(milaX, milaY, NULL, degrees, NULL, fliptype);

				gSprites->render(SCREEN_WIDTH - gDrawingSpriteClips[0].w, SCREEN_HEIGHT - gDrawingSpriteClips[0].h, &gDrawingSpriteClips[0]);

				SDL_Rect* currentClip = &gSpriteClips[frame / 4];
				SDL_Rect wall = { 100,100,30,200 };
				gWalkingAnim->render(SCREEN_WIDTH - currentClip->w, 0, currentClip);


				gDot->move(wall);
				gDot->render();

				SDL_SetRenderDrawColor(gRenderer, 50, 50,50, 0xFF);
				SDL_RenderFillRect(gRenderer, &wall);
				//draw buttons events
				for (size_t i = 0; i < TOTAL_BUTTONS; i++) {

					//only draw bottom right buttonfor now
					gButtons[3]->render();
					
				}

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

	return 0;
}


void close()
{
	for (size_t i = 0; i < TEXTURES_SIZE; i++)
	{
		delete gTextures[i];
		gTextures[i] = NULL;
	}

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

	delete gText;
	gText = NULL;

	delete gFPSText;
	gFPSText = NULL;

	delete gTexButtons;
	gTexButtons = NULL;

	TTF_CloseFont(gBigFont);
	TTF_CloseFont(gSmallFont);
	gBigFont = NULL;
	gSmallFont = NULL;

	SDL_HapticClose(gControllerHaptic);
	SDL_JoystickClose(gGameController);
	gGameController = NULL;
	gControllerHaptic = NULL;

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

