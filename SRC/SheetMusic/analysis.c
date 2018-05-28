
#include <stdio.h>
#include <stdlib.h>

#include "analysis.h"

void clearChords(SheetMusic *sheet, int staffIdx)
{
    Note *notes;
    int nbNotes, newNbNotes;
    int i;

    if (!sheet)
        return;
    if (staffIdx >= sheet->nbStaves)
        return;

    notes = sheet->notes[staffIdx];
    nbNotes = sheet->nbNotes[staffIdx];

    newNbNotes = 1;
    for (i = 1; i < nbNotes; i++)
    {
        if (notes[i].appearingBeat - notes[i - 1].appearingBeat >= 0.0625)
        {
            notes[newNbNotes] = notes[i];
            newNbNotes++;
        }
    }

    sheet->nbNotes[staffIdx] = newNbNotes;
}

void placeNotes(SheetMusic *sheet)
{
    int i;
    int nbStrings;

    if (!sheet)
        return;

    nbStrings = sheet->nbStrings;

    for (i = 0; i < sheet->nbStaves; i++)
    {
        clearChords(sheet, i);
        placeNotesStaff(sheet->notes[i], sheet->nbNotes[i], nbStrings);
    }
}

void placeNotesStaff(Note * notes, int nbNotes, int nbStrings)
{

  int i = 0;
  int CurrentString = nbStrings/2;
  for(i = 1 ;  i < nbNotes ; i++)
  {

      if(notes[i].value < notes[i-1].value)
      {

        if(CurrentString = 0)
        {

          notes[i].stringIdx = 0;

        }

        else
        {

          CurrentString--;
          notes[i].stringIdx = CurrentString;

        }
      }

      if(notes[i].value > notes[i-1].value)
      {

        if(CurrentString = nbStrings - 1)
        {

          notes[i].stringIdx = nbStrings - 1;

        }

        else
        {

          CurrentString++;
          notes[i].stringIdx = CurrentString;

        }
      }

      else
      {

        notes[i].stringIdx = CurrentString;
        
      }
  }
}
