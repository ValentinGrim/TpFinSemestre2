
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
    // Remise à zéro de l'affichage

    SDL_RenderClear(renderer);

    //******************************************************************************************************************
    // Fond de la guitare

    x = metrics->gameArea.x;
    y = metrics->gameArea.y;
    renderTexture(textures->gameArea, renderer, x, y);

    //******************************************************************************************************************
    // Notes

    curNote = &notes[0];
    stringIdx = curNote->stringIdx;
    x = metrics->gameArea.x + (metrics->gameArea.w - metrics->note.w) / 2;
    y = metrics->gameArea.y + curNote->relPos * metrics->gameArea.h;
    renderTexture(textures->notes[stringIdx], renderer, x, y);

    //******************************************************************************************************************
    // Mise à jour de l'affichage

    SDL_RenderPresent(renderer);
}
