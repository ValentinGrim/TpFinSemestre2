#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gameMetrics.h"
#include "../tools.h"


Metrics * newMetrics(Textures * textures)
{
    Metrics * metrics = NULL;
    int w, h;

    if (!textures)
        return NULL;

    metrics = (Metrics *)calloc(1, sizeof(Metrics));
    if (!metrics)
        return NULL;

    metrics->screen.x = 0;
    metrics->screen.y = 0;
    metrics->screen.w = WINDOW_WIDTH;
    metrics->screen.h = WINDOW_HEIGHT;

    SDL_QueryTexture(textures->notes[0], NULL, NULL, &w, &h);
    metrics->note.x = 0;
    metrics->note.y = 0;
    metrics->note.w = w;
    metrics->note.h = h;

    SDL_QueryTexture(textures->gameArea, NULL, NULL, &w, &h);
    metrics->gameArea.x = (metrics->screen.w - w) / 2;
    metrics->gameArea.y = (metrics->screen.h - h) / 2;
    metrics->gameArea.w = w;
    metrics->gameArea.h = h;

		SDL_QueryTexture(textures->string, NULL, NULL, &w, &h);
		metrics->string.x = (metrics->screen.w - w)/2;
		metrics->string.y = (metrics->screen.h - h)/2;
		metrics->string.w = w;
		metrics->string.h = h;

    return metrics;
}

void freeMetrics(Metrics * metrics)
{
    free(metrics);
}

void initMetrics(Metrics * metrics, int nbStrings)
{
    // TODO : calculer les positions des �l�ments en fonction du nombre de cordes
/*		switch (nbStrings)
		{

			case 4:

			case 5:

				break;
		}*/

}
