
#ifndef _MAIN_WINDOW_INCLUDED_
#define _MAIN_WINDOW_INCLUDED_

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

typedef struct MainWindow_s
{
    SDL_Window   *window;
    SDL_Renderer *renderer;
} MainWindow;

MainWindow * newMainWindow();
void freeMainWindow(MainWindow * mainWindow);

int initSDL();
void quitSDL();

#endif
