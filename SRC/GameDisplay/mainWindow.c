#ifdef _WIN32
	#include <SDL.h>
	#include <SDL_image.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mainWindow.h"

MainWindow * newMainWindow()
{
    MainWindow *mainWindow = NULL;

    mainWindow = (MainWindow *)calloc(1, sizeof(MainWindow));
    if (!mainWindow)
        return NULL;

    //mainWindow->window = SDL_CreateWindow("Teubies Legend", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0);
    mainWindow->window = SDL_CreateWindow( "Midi Legend", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN );
    if (!mainWindow->window)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        goto ERROR_LABEL;
    }

    mainWindow->renderer = SDL_CreateRenderer(mainWindow->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mainWindow->renderer)
    {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        goto ERROR_LABEL;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(mainWindow->renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetRenderDrawColor(mainWindow->renderer, 0, 0, 0, 255);

    return mainWindow;

ERROR_LABEL:
    free(mainWindow);
    return NULL;
}

void freeMainWindow(MainWindow * mainWindow)
{
    if (mainWindow)
    {
        if (mainWindow->window)
        {
            SDL_DestroyWindow(mainWindow->window);
        }
        if (mainWindow->renderer)
        {
            SDL_DestroyRenderer(mainWindow->renderer);
        }
        free(mainWindow);
    }
}

int initSDL()
{
    // Initialisation de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Initialisation de SDL Image
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void quitSDL()
{
    SDL_Quit();
    IMG_Quit();
}
