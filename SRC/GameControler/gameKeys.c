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

    if (pianoMode==1)
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

void processGameEvents(SDLGameConfig * config, GameKeys * gameKeys, char * nomfichier, int *  highScores, int * points)
{
    SDL_Event evt;
    SDL_Scancode scanCode;
    int stringIdx;

    gameKeys->quitDown = 0;
    gameKeys->strumDown = 0;
    gameKeys->exitDown = 0;

    //******************************************************************************************************************
    // Parcourt des events

    // TODO : G�rer les actions de l'utilisateur

    while ( SDL_PollEvent( &evt ) )
    {
        switch ( evt.type )
        {
        case SDL_QUIT: //......................................................................... La fen�tre est ferm�e
            gameKeys->quitDown = 1;
            break;

        case SDL_KEYDOWN: //..................................................................... Une touche est appuy�e
            scanCode = evt.key.keysym.scancode;

						for(int i = 0 ; i < MAX_STRINGS ; i++)
						{

							if(scanCode == config->fretValues[i])
							{

								gameKeys->fretDown[i] = 1;

							}


						}

						if (scanCode == config->strumValue)
  							gameKeys->strumDown = 1;


            if ( evt.key.repeat )
                break;

            // Touche retour vers le menu
            if ( scanCode == config->exitValue )
            {
								FILE * pFichier = NULL;
								pFichier = fopen(nomfichier, "w");

								for(int i = 0; i < 3; i++)
								{
									if(highScores[i] < points)
									{
										if(i + 1 < 3)
										{
											if(i + 2 < 3)
											{
												highScores[i+2] = highScores[i+1];
											}
											highScores[i+1] = highScores[i];
										}
										highScores[i] = points;
										i = 3;
									}
								}
								fprintf(pFichier,"1 - %d\n", highScores[0]);
								fprintf(pFichier,"2 - %d\n", highScores[1]);
								fprintf(pFichier,"3 - %d\n", highScores[2]);
                gameKeys->exitDown = 1;
            }
            break;

        case SDL_KEYUP: //...................................................................... Une touche est relach�e

						scanCode = evt.key.keysym.scancode;
						for(int i = 0 ; i < MAX_STRINGS ; i++)
						{

							if(scanCode == config->fretValues[i])
							{

								gameKeys->fretDown[i] = 0;

							}

        	}
    	}
		}
}
