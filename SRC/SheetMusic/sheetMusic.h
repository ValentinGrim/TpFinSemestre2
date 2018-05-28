
#ifndef SCORE_H
#define SCORE_H

#include "midiFile.h"
#include <stdio.h>

typedef struct Note_s
{
    int   value;
    int   stringIdx;
    float appearingBeat;
    float durationBeat;
    float appearingTime;
    float durationTime;
} Note;

typedef struct Tempo_s
{
    int msPerBeat;
    float appearingBeat;
    float appearingTime;
} Tempo;

typedef struct SheetMusic_s
{
    int    nbStrings;
    int    nbStaves; // Nombre de portées
    int   *nbNotes;
    Note **notes;

    int    nbTempos; // Nombre de variation de tempo
    Tempo *tempos;
} SheetMusic;

SheetMusic* newSheetMusic(FILE *midiFile);
void freeSheetMusic(SheetMusic *sheet);

void fprintNoteValue(FILE *file, Note *note);
void fprintStaff(FILE *file, SheetMusic *sheet, int staffIdx);
void fprintSheetMusic(FILE *file, SheetMusic *sheet);

void printNoteValue(Note *note);
void printStaff(SheetMusic *sheet, int staffIdx);
void printSheetMusic(SheetMusic *sheet);

#endif