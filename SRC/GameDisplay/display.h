
#ifndef _DISPLAY_INCLUDED_
#define _DISPLAY_INCLUDED_

#include "../GameModel/model.h"
#include "mainWindow.h"
#include "textures.h"
#include "gameMetrics.h"

#ifdef _WIN32
#include <Windows.h>
#include <SDL.h>
#include <SDL_mixer.h>
#else
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

typedef struct GameDisplay_s
{
    Textures  * textures;   // Ensemble des elements graphiques pre-chargés
    Metrics   * metrics;    // Positions des elements graphiques constants
} GameDisplay;

GameDisplay * newGameDisplay(MainWindow *mainWindow);
void freeGameDisplay(GameDisplay * gameDisp);

void updateGameDisplay(GameDisplay *gameDisp, MainWindow *mainWindow, Model *model);

typedef struct
{
    float t;
    Mix_Music *music;
} MusicThread;

#endif
