
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
    SDL_Texture *notes[MAX_STRINGS];// tableau qui contient l image de "voir en début de ligne"
		SDL_Texture *background; // tableau qui contient l image de "voir en début de ligne"
    SDL_Texture *gameArea;// tableau qui contient l image de "voir en début de ligne"
		SDL_Texture *string;// tableau qui contient l image de "voir en début de ligne"
		SDL_Texture *strum[MAX_STRINGS];// tableau qui contient l image de "voir en début de ligne"
		SDL_Texture *strum_held[MAX_STRINGS];// tableau qui contient l image de "voir en début de ligne"
		SDL_Texture *combo;// tableau qui contient l image de "voir en début de ligne"
		SDL_Texture *chiffres[MAX_COMBO];// tableau qui contient l image de "voir en début de ligne"
		SDL_Texture *score[MAX_COMBO];// tableau qui contient l image de "voir en début de ligne"
		SDL_Texture *backHighscores;// tableau qui contient l image de "voir en début de ligne"

} Textures;

Textures * newTextures(SDL_Renderer *renderer);
void freeTextures(Textures *textures);

SDL_Texture* loadTexture(char *imgPath, SDL_Renderer *renderer);
void destroyTexture(SDL_Texture * texture);

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);

#endif
