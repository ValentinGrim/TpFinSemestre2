
#ifndef _GAME_METRICS_INCLUDED_
#define _GAME_METRICS_INCLUDED_

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "../GameModel/model.h"
#include "textures.h"

typedef struct Metrics_s
{
    SDL_Rect screen;
    SDL_Rect note;
    SDL_Rect gameArea;
    SDL_Rect *string;
    SDL_Rect *strum;
    SDL_Rect *strum_held;
} Metrics;



Metrics * newMetrics(Textures * textures);
void freeMetrics(Metrics * metrics);

void initMetrics(Metrics *metrics, int nbStrings);

#endif
