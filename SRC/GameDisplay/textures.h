
#ifndef _TEXTURES_INCLUDED_
#define _TEXTURES_INCLUDED_

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "mainWindow.h"
#include "../GameModel/model.h"

typedef struct Textures_s
{
    SDL_Texture *notes[MAX_STRINGS];
		SDL_Texture *background;
    SDL_Texture *gameArea;
		SDL_Texture *string;
		SDL_Texture *strum[MAX_STRINGS];
		SDL_Texture *strum_held[MAX_STRINGS];
		SDL_Texture *combo;
		SDL_Texture *chiffres[MAX_COMBO];
		SDL_Texture *score[MAX_COMBO];
} Textures;

Textures * newTextures(SDL_Renderer *renderer);
void freeTextures(Textures *textures);

SDL_Texture* loadTexture(char *imgPath, SDL_Renderer *renderer);
void destroyTexture(SDL_Texture * texture);

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);

#endif
