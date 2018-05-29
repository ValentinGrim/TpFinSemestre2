
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../SheetMusic/analysis.h"
#include "../SheetMusic/midiFile.h"
#include "../SheetMusic/sheetMusic.h"
#include "../GameModel/model.h"
#include "../tools.h"
#include "display.h"

#include "mainWindow.h"
#include "textures.h"

GameDisplay * newGameDisplay(MainWindow * mainWindow)
{
    GameDisplay * gameDisp = NULL;

    if (!mainWindow)
        return NULL;

    gameDisp = (GameDisplay *)calloc(1, sizeof(GameDisplay));
    if (!gameDisp)
        goto ERROR_LABEL;

    gameDisp->textures = newTextures(mainWindow->renderer);
    if (!gameDisp->textures)
        goto ERROR_LABEL;
    gameDisp->metrics = newMetrics(gameDisp->textures);
    if (!gameDisp->metrics)
        goto ERROR_LABEL;

    return gameDisp;

ERROR_LABEL:
    freeGameDisplay(gameDisp);
    return NULL;
}

void freeGameDisplay(GameDisplay * gameDisp)
{
    if (gameDisp)
    {
        freeTextures(gameDisp->textures);
        freeMetrics(gameDisp->metrics);

        free(gameDisp);
    }
}

void updateGameDisplay(GameDisplay *gameDisp, MainWindow *mainWindow, Model *model)
{
    SDL_Renderer *renderer = mainWindow->renderer;
    Textures *textures = gameDisp->textures;
    GameSheet *gameSheet = model->gameSheet;
    GameNote *notes, *curNote;
    Metrics *metrics;
    int x, y;
    int stringIdx, staffIdx;

    staffIdx = gameSheet->staffIdx;
    notes = gameSheet->notes[staffIdx];
    metrics = gameDisp->metrics;

    //******************************************************************************************************************
    // Remise � z�ro de l'affichage

    SDL_RenderClear(renderer);

    //******************************************************************************************************************
    // Fond d'écran

    x = metrics->screen.x;
    y = metrics->screen.y;
    renderTexture(textures->background, renderer, x, y);

    //******************************************************************************************************************
    // Fond de la guitare

    x = metrics->gameArea.x;
    y = metrics->gameArea.y;
    renderTexture(textures->gameArea, renderer, x, y);

    //******************************************************************************************************************
    // COOOOOOOOOORDES
    for(int i=0; i<gameSheet->nbStrings; i++)
    {
      x = metrics->string[i].x;
      y = metrics->string[i].y;
      renderTexture(textures->string, renderer, x, y);
    }

    //******************************************************************************************************************
    // Fond de la strum
    for(int i=0; i<gameSheet->nbStrings; i++)
    {
      x = metrics->strum[i].x;
      y = metrics->strum[i].y;

      if (model->keys->fretDown[i]==1)
        {
            renderTexture(textures->strum_held[i], renderer, x, y);
        }
      else
        {
            renderTexture(textures->strum[i], renderer, x, y);
        }

    }

    //******************************************************************************************************************
    // Notes
    for(int i = 0; i < gameSheet->nbNotes[staffIdx]; i++)
    {

      if(notes[i].visible == 1)
      {
        curNote = &notes[i];
        stringIdx = curNote->stringIdx;
        x = ((metrics->gameArea.w)/(gameSheet->nbStrings + 1))*(notes[i].stringIdx+1)+(metrics->gameArea.x)*0.95;
        y = metrics->gameArea.y + curNote->relPos * metrics->gameArea.h;
        renderTexture(textures->notes[stringIdx], renderer, x,y);
      }
    }
    /*curNote = &notes[0];
    stringIdx = curNote->stringIdx;
    x = metrics->gameArea.x + (metrics->gameArea.w - metrics->note.w) / 2;
    y = metrics->gameArea.y + curNote->relPos * metrics->gameArea.h;
    renderTexture(textures->notes[stringIdx], renderer, x, y);*/

    //******************************************************************************************************************
    // Mise � jour de l'affichage

    SDL_RenderPresent(renderer);
}
