//Using SDL, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include "note.h"
#include "note_image.h"
#include "scene.h"
#include "music.h"
#include "const.h"
#include "lane.h"
#include "beatmap.h"

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( const char * path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

bool loadImages();
SDL_Texture *arrowImage;

Scene *mainScene = NULL;
Music *music = NULL;
Lane *testLane = NULL;
std::vector<Lane *> lanes;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }

				if(!loadImages()){
					success = false;
				}

				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//create scene
				mainScene = new Scene(gRenderer);

				//create music
				music = new Music("SmashMouth-AllStar.ogg");

				//test lane
				for(int i = 0; i < 4; i++){
					Lane *newLane = new Lane(gRenderer, music, arrowImage, 100 + 110 * i);
					mainScene->push_back(newLane);
					lanes.push_back(newLane);
				}
			}
		}
	}

	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	delete music;
	music = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Surface* loadSurface( const char * path )
{
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError() );
	}

	return loadedSurface;
}

bool loadImages()
{
	bool success = true;
	
	SDL_Surface *surface = loadSurface("arrow.bmp");
	if(surface == NULL){
		success = false;
	}else{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB( surface->format, 0xff, 0xff, 0xff));
		arrowImage = SDL_CreateTextureFromSurface(gRenderer, surface);
	}

	return success;
}

//loop for creating a beatmap
void create_loop() {
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	music->play();

	std::vector<std::vector<Note *>> lanes(4, std::vector<Note *>());

	//While application is running
	while( !quit )
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
			//User presses a key
			else if( e.type == SDL_KEYDOWN )
			{
				int laneNo = -1;
				//Select surfaces based on key press
				switch( e.key.keysym.sym )
				{
					case SDLK_UP: laneNo = 2; break;
					case SDLK_DOWN: laneNo = 1; break;
					case SDLK_LEFT: laneNo = 0; break;
					case SDLK_RIGHT: laneNo = 3; break;
					default: break;
				}
				if(laneNo != -1) {
					lanes[laneNo].push_back(new Note(music->getSeconds()));
					printf("%lf\n", music->getSeconds());
				}
			}
		}

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		mainScene->render();

		//Update screen
		SDL_RenderPresent( gRenderer );
	}

	writeBeatMap("all_star.map", lanes);
}

void game_loop() {
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	music->play();

	//While application is running
	while( !quit )
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
			//User presses a key
			else if( e.type == SDL_KEYDOWN )
			{
				int laneNo = -1;
				//Select surfaces based on key press
				switch( e.key.keysym.sym )
				{
					case SDLK_UP: laneNo = 2; break;
					case SDLK_DOWN: laneNo = 1; break;
					case SDLK_LEFT: laneNo = 0; break;
					case SDLK_RIGHT: laneNo = 3; break;
					default: break;
				}
				if(laneNo != -1) {
					lanes[laneNo]->hit();
				}
			}
		}

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		for(auto lane : lanes) {
			lane->updateViewable();
		}
		mainScene->render();

		//Update screen
		SDL_RenderPresent( gRenderer );
	}
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		readBeatMap("test_all_star.map", lanes);
		game_loop();
	}

	//Free resources and close SDL
	close();

	return 0;
}