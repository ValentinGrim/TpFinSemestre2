
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
    int xXcOmbOXx = 10;
    int cOmbOMult;

    int nbNotes=*(model->gameSheet->nbNotes),i;
    GameNote *notes;

    int string;

    int staffIdx = model->gameSheet->staffIdx;
    notes = model->gameSheet->notes[staffIdx];

    for(i=0;i<nbNotes;i++)
     {

       switch(xXcOmbOXx/10)
       {
         case 0:
           cOmbOMult = 1;
           break;

         case 1:
           cOmbOMult = 2;
           break;

         case 2:
           cOmbOMult = 4;
           break;

         case 3:
           cOmbOMult = 10;
           break;

         case 4:
           cOmbOMult = 20;
           break;

         case 5:
           cOmbOMult = 20;
           break;

         case 6:
           cOmbOMult = 20;
           break;

         case 7:
           cOmbOMult = 20;
           break;

         case 8:
           cOmbOMult = 20;
           break;

         case 9:
           cOmbOMult = 20;
           break;

         case 10:
           cOmbOMult = 50;
           break;

         default:
           cOmbOMult = 50;

       }

     	if(notes[i].visible==1&& notes[i].state!=statePlayed)
     	{
     		string=notes[i].stringIdx;

     		if((notes[i].playingTime <= model->timer->currentTime+0.09) &&(notes[i].playingTime >= model->timer->currentTime-0.09) && (model->keys->fretDown[string]==1))
     		{

          xXcOmbOXx++;
     			notes[i].state=statePlayed;
     			notes[i].visible=0;
     			model->points+=(50 * cOmbOMult);
     			printf("points:%d  %d\n",(i+1)*100, model->points);
     		}
        if(notes[i].playingTime+0.09 < model->timer->currentTime && notes[i].state == stateAlive)
        {

          if(xXcOmbOXx > 40)
          {

            xXcOmbOXx = 40;

          }

          xXcOmbOXx--;
          notes[i].state = stateFailed;
          model->points-= (10 * cOmbOMult);
          printf("Note %d raté, points : %d\n", i+1, model->points);

        }
     	}
     }


}

Model * newModel(SheetMusic * sheet, float relSpeed)
{
    Model * model = NULL;

    if (!sheet)
        return NULL;

    model = (Model *)calloc(1, sizeof(Model));

    model->keys = newGameKeys();
    model->gameSheet = newGameSheet(sheet);
    model->timer = newTimer(relSpeed);
    model->points = 0.0f;

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
