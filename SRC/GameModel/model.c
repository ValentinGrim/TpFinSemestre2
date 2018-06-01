
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "model.h"
#include "timer.h"

GameSheet * newGameSheet(SheetMusic * sheet)
{
    GameSheet * gameSheet = NULL;
    Note * note = NULL;
    GameNote * gameNote = NULL;
    int nbStaves;
    int i, noteIdx;

    if (!sheet)
        return NULL;

    gameSheet = (GameSheet *)calloc(1, sizeof(GameSheet)); nbStaves = sheet->nbStaves;

    gameSheet->nbStaves = nbStaves;
    gameSheet->nbStrings = sheet->nbStrings;
    gameSheet->nbNotes = (int *)calloc(nbStaves, sizeof(int));
    gameSheet->notes = (GameNote **)calloc(nbStaves, sizeof(GameNote *));

    memcpy(gameSheet->nbNotes, sheet->nbNotes, nbStaves * sizeof(int));

    for (i = 0; i < nbStaves; i++)
    {
        gameSheet->notes[i] = (GameNote *)calloc(sheet->nbNotes[i], sizeof(GameNote));
    }

    gameSheet->staffIdx = 0;

    for (i = 0; i < nbStaves; i++)
    {
        for (noteIdx = 0; noteIdx < sheet->nbNotes[i]; noteIdx++)
        {
            note = sheet->notes[i] + noteIdx;
            gameNote = gameSheet->notes[i] + noteIdx;

            gameNote->value = note->value;
            gameNote->stringIdx = note->stringIdx;
            gameNote->playingTime = note->appearingTime;
            gameNote->duration = note->durationTime;
            gameNote->endingTime = note->appearingTime + note->durationTime;
            gameNote->visible = 1;
            gameNote->state = stateAlive;
            gameNote->relPos = 0.0f;

            if (gameNote->duration > MINIMAL_LONG_NOTE)
                gameNote->type = typeLongNote;
            else
                gameNote->type = typeShortNote;
        }
    }

    return gameSheet;
}

void freeGameSheet(GameSheet * gameSheet)
{
    int i;

    if (gameSheet)
    {
        if (gameSheet->notes)
        {
            for (i = 0; i < gameSheet->nbStaves; i++)
                free(gameSheet->notes[i]);
        }
        free(gameSheet->notes);
        free(gameSheet->nbNotes);
    }
    free(gameSheet);
}

void updateGameSheet(GameSheet *sheet, Timer *timer)
{
    int staffIdx, i;
    GameNote *notes;

    staffIdx = sheet->staffIdx;
    notes = sheet->notes[staffIdx];

    // TODO : Faire descendre les notes
    // Voici une d�mo

    for(i = 0; i < sheet->nbNotes[staffIdx]; i++)
    {
      float fallTime = notes[i].playingTime - timer->timeBeforeStrum;
      float falledTime = notes[i].playingTime + timer->timeAfterStrum - 0.1;
      if(fallTime < timer->currentTime && notes[i].state == stateAlive)
      {

        notes[i].visible = 1;
        notes[i].relPos += timer->delta * timer->relSpeed;

      }
      if(falledTime < timer->currentTime)
      {

        notes[i].visible = 0;

      }

      if(fallTime > timer->currentTime && falledTime > timer->currentTime)
      {

        notes[i].visible = 0;

      }
    }
}

void checkStrum(Model *model)
{
    // TODO : Gérer les actions du joueur et modifier l'état des notes

    if(model->xXcOmbOXx < 10)
    {

      model->cOmbOMult = 1;

    }

    if(model->xXcOmbOXx >= 10 && model->xXcOmbOXx < 20)
    {

      model->cOmbOMult = 2;

    }
    if(model->xXcOmbOXx >= 20 && model->xXcOmbOXx < 30)
    {

      model->cOmbOMult = 3;

    }
    if(model->xXcOmbOXx >= 30 && model->xXcOmbOXx < 40)
    {

      model->cOmbOMult = 4;

    }
    if(model->xXcOmbOXx >= 40 && model->xXcOmbOXx < 100)
    {

      model->cOmbOMult = 5;

    }
    if(model->xXcOmbOXx >= 100)
    {

      model->cOmbOMult = 10;

    }

    int nbNotes=*(model->gameSheet->nbNotes),i;
    GameNote *notes;

    int string;

    int staffIdx = model->gameSheet->staffIdx;
    notes = model->gameSheet->notes[staffIdx];

    for(i=0;i<nbNotes;i++)
     {

     	if(notes[i].visible==1 && notes[i].state!=statePlayed)
     	{
     		string=notes[i].stringIdx;
        if (model->keys->pianoMode !=1)
        {
          if((notes[i].playingTime <= model->timer->currentTime+0.09) &&(notes[i].playingTime >= model->timer->currentTime-0.09) && (model->keys->fretDown[string]==1))
       	  {

            model->xXcOmbOXx++;
            model->life ++;

       			notes[i].state=statePlayed;
       			notes[i].visible=0;

            if(model->cheatMode == 0)
            {
                model->keys->fretDown[string] = 1;
            }

       			model->points+=(100 * model->cOmbOMult);
       		}
        }

        else
        {
          if((notes[i].playingTime <= model->timer->currentTime+0.09) &&(notes[i].playingTime >= model->timer->currentTime-0.09) && (model->keys->fretDown[string]==1) && ((model->keys->strumDown ==1)||(model->keys->strumDown == 2)))
          {

            model->xXcOmbOXx++;
            model->life ++;

            notes[i].state=statePlayed;
            notes[i].visible=0;

            if(model->cheatMode == 0)
            {
                model->keys->fretDown[string] = 1;
            }

            model->points+=(100 * model->cOmbOMult);
          }
        }
    }



    }
    if(notes[i].playingTime+0.09 < model->timer->currentTime && notes[i].state == stateAlive)
      {

        model->xXcOmbOXx = 0;
        model->life--;

        if(model->points > 10)
        {

          model->points-= 10;

        }
      }
      TabPoints(model);
     }




void TabPoints(Model *model)
{

  int points = model->points;
  int TabTemp[7];

  for(int i = 0; i < 7; i++)
  {

    int tmp = points % 10;
    points /= 10;
    TabTemp[i] = tmp;
    model->pointTab[6-i] = TabTemp[i];

  }

}

void TabHighscores(Model *model)
{

  int points = model->points;
  int TabTemp[7];

  for (int j = 0; j < 3; j++)
  {
    int points = model->highScores[j];
    for(int i = 0; i < 7; i++)
    {

      int tmp = points % 10;
      points /= 10;
      TabTemp[i] = tmp;
      model->highScoresTab[j][6-i] = TabTemp[i];
      printf("%d ", model->highScoresTab[j][i]);


    }
  }
  printf("\n");

}

Model * newModel(SheetMusic * sheet, float relSpeed, char * arg)
{

    Model * model = NULL;

    if (!sheet)
        return NULL;

    model = (Model *)calloc(1, sizeof(Model));

    model->keys = newGameKeys();
    model->gameSheet = newGameSheet(sheet);
    model->timer = newTimer(relSpeed);
    model->points = 0.0f;
    model->cOmbOMult = 1;
    model->life = 5;
    model->xXcOmbOXx = 0;
    model->cheatMode = 1;

    FILE *pFichier = NULL;
    char * PointerSurArgu = model->nomfichier;
    memmove(arg, arg+8, strlen(arg));
    sprintf(PointerSurArgu, "scores_%s.txt", arg);
    pFichier = fopen(model->nomfichier, "r");
    if(!pFichier)
    {

      pFichier = fopen(model->nomfichier, "w");
      fprintf(pFichier,"1 - 0\n2 - 0\n3 - 0\n");

    }

    else
    {


      fscanf(pFichier,"1 - %d\n", &model->highScores[0]);
      fscanf(pFichier,"2 - %d\n", &model->highScores[1]);
      fscanf(pFichier,"3 - %d", &model->highScores[2]);

    }

    for(int i = 0; i < 7; i++)
    {

      model->pointTab[i] = 0;

    }
    TabHighscores(model);
    fclose(pFichier);
    return model;
}

void freeModel(Model * model)
{
    if (model)
    {
        freeGameKeys(model->keys);
        freeGameSheet(model->gameSheet);
        freeTimer(model->timer);
    }
    free(model);
}

void updateModel(Model *model)
{
    Timer * timer = model->timer;
    GameSheet * gameSheet = model->gameSheet;

    updateTimer(timer);
    updateGameSheet(gameSheet, timer);
    checkStrum(model);
}
