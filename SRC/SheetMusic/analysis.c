
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
  for(int i = 1 ; i < nbNotes ; i++)
  {

    if(notes[i].value < notes[i-1].value)
    {

      if(notes[i-1].stringIdx <= 0)
      {

        notes[i].stringIdx = notes[i-1].stringIdx;

      }

      else
      {

        notes[i].stringIdx = notes[i-1].stringIdx - 1;

      }

    }

    else if(notes[i].value > notes[i-1].value)
    {

      if(notes[i-1].stringIdx >= nbStrings - 1)
      {

        notes[i].stringIdx = notes[i-1].stringIdx;

      }

      else
      {

        notes[i].stringIdx = notes[i-1].stringIdx + 1;

      }

    }

    else
    {

      notes[i].stringIdx = notes[i-1].stringIdx;

    }
  }
}
