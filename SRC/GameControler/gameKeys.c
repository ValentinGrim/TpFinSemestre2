#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../GameModel/model.h"
#include "gameKeys.h"


SDLGameConfig * newGameConfig(int pianoMode)
{
    SDLGameConfig * config = NULL;
    config = (SDLGameConfig *)calloc(1, sizeof(SDLGameConfig));
    if (!config)
        return NULL;

    config->pianoMode = pianoMode;

    if (pianoMode)
    {
        config->fretValues[0] = SDL_SCANCODE_Q;
        config->fretValues[1] = SDL_SCANCODE_W;
        config->fretValues[2] = SDL_SCANCODE_E;
        config->fretValues[3] = SDL_SCANCODE_R;
        config->fretValues[4] = SDL_SCANCODE_SPACE;
    }
    else
    {
        config->fretValues[0] = SDL_SCANCODE_F1;
        config->fretValues[1] = SDL_SCANCODE_F2;
        config->fretValues[2] = SDL_SCANCODE_F3;
        config->fretValues[3] = SDL_SCANCODE_F4;
        config->fretValues[4] = SDL_SCANCODE_F5;
    }

    config->strumValue = SDL_SCANCODE_RETURN;
    config->exitValue = SDL_SCANCODE_ESCAPE;

    return config;
}

void freeGameConfig(SDLGameConfig * config)
{
    free(config);
}

GameKeys * newGameKeys()
{
    GameKeys * gameKeys = NULL;
    gameKeys = (GameKeys *)calloc(1, sizeof(GameKeys));
    return gameKeys;
}

void freeGameKeys(GameKeys * gameKeys)
{
    free(gameKeys);
}

void processGameEvents(SDLGameConfig * config, GameKeys * gameKeys)
{
    SDL_Event evt;
    SDL_Scancode scanCode;
    int stringIdx;

    gameKeys->quitDown = 0;
    gameKeys->strumDown = 0;
    gameKeys->exitDown = 0;

    //******************************************************************************************************************
    // Parcourt des events

    // TODO : Gérer les actions de l'utilisateur

    while ( SDL_PollEvent( &evt ) )
    {
        switch ( evt.type )
        {
        case SDL_QUIT: //......................................................................... La fenêtre est fermée
            gameKeys->quitDown = 1;
            break;

        case SDL_KEYDOWN: //..................................................................... Une touche est appuyée
            scanCode = evt.key.keysym.scancode;

            if ( evt.key.repeat )
                break;

            // Touche retour vers le menu
            if ( scanCode == config->exitValue )
            {
                gameKeys->exitDown = 1;
            }
            break;

        case SDL_KEYUP: //...................................................................... Une touche est relachée
            scanCode = evt.key.keysym.scancode;
            break;
        }
    }
}
