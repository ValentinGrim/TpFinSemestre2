
#ifndef _GAME_KEYS_INCULDED_
#define _GAME_KEYS_INCULDED_

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "../GameModel/model.h"

typedef struct SDLGameConfig_s
{
    int pianoMode;
    // Touches du clavier menant a une action
    SDL_Scancode exitValue;
    SDL_Scancode strumValue;
    SDL_Scancode fretValues[MAX_STRINGS];
} SDLGameConfig;

SDLGameConfig * newGameConfig(int pianoMode);
void freeGameConfig(SDLGameConfig * config);

typedef struct GameKeys_s
{
    int exitDown;  // On retourne au menu
    int strumDown; //variable touche entrée activée
    int fretDown[MAX_STRINGS]; //tableau touche cordes
    int quitDown;  // On ferme fenetre
} GameKeys;

GameKeys * newGameKeys();
void freeGameKeys(GameKeys * gameKeys);

void processGameEvents(SDLGameConfig * config, GameKeys * gameKeys, char * nomfichier, int *  highScores, int points);

#endif
