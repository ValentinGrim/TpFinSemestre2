
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "midiFile.h"
#include "sheetMusic.h"

float midiTimeToBeat(int midiTime, Midi *midi)
{
    return (float)midiTime / (float)midi->division;
}

SheetMusic* newSheetMusic(FILE *midiFile)
{
    SheetMusic *sheet = NULL;
    int trackIdx, channelIdx, staffIdx, noteIdx, noteOnIdx;
    int tempoIdx;
    int nbNotes, nbTracks, nbStaves;
    int *staffToTrack = NULL, *staffToChannel = NULL;
    int i;
    int *curNotesOn = NULL;
    int msPerBeat;
    float preBeat, curBeat;
    float curTime, preTime;
    NoteEvt *noteEvt = NULL;
    Midi *midi = NULL;

    if (!midiFile)
        return NULL;

    // Lecture du fichier midi
    midi = newMidi(midiFile);

    sheet = (SheetMusic *)calloc(1, sizeof(SheetMusic));

    //******************************************************************************************************************
    // Création des changements de tempo

    sheet->nbTempos = midi->nbTempos;
    sheet->tempos = (Tempo *)calloc(midi->nbTempos, sizeof(Tempo));

    curTime = 0.0;
    msPerBeat = 500000;
    preBeat = 0.0;
    for (i = 0; i < midi->nbTempos; i++)
    {
        curBeat = midiTimeToBeat(midi->tempos[i].apearingMidiTime, midi);
        curTime += (curBeat - preBeat) * (float)msPerBeat / 1000000.0f;

        sheet->tempos[i].appearingBeat = curBeat;
        sheet->tempos[i].appearingTime = curTime;
        sheet->tempos[i].msPerBeat = midi->tempos[i].msPerBeat;
        msPerBeat = midi->tempos[i].msPerBeat;

        preBeat = curBeat;
    }

    //******************************************************************************************************************
    // Création des portées

    // Allocations
    nbTracks = midi->nbTracks;
    staffToTrack = (int *)calloc(16 * nbTracks, sizeof(int));
    staffToChannel = (int *)calloc(16 * nbTracks, sizeof(int));
    sheet->nbNotes = (int *)calloc(16 * nbTracks, sizeof(int));

    staffIdx = 0;
    for (trackIdx = 0; trackIdx < midi->nbTracks; trackIdx++)
    {
        for (channelIdx = 0; channelIdx < 16; channelIdx++)
        {
            // Calcul du nombre de notes dans la portée
            nbNotes = 0;
            for (i = 0; i < midi->nbNotes[trackIdx]; i++)
            {
                if ((midi->notes[trackIdx][i].channel == channelIdx) &&
                    (midi->notes[trackIdx][i].velocity > 0))
                {
                    nbNotes++;
                }
            }

            if (nbNotes > 0)
            {
                staffToTrack[staffIdx] = trackIdx;
                staffToChannel[staffIdx] = channelIdx;
                sheet->nbNotes[staffIdx] = nbNotes;
                staffIdx++;
            }
        }
    }
    nbStaves = staffIdx;
    sheet->nbStaves = nbStaves;

    // Allocation des portées
    sheet->notes = (Note **)calloc(nbStaves, sizeof(Note *));
    for (i = 0; i < nbStaves; i++)
    {
        sheet->notes[i] = (Note *)calloc(sheet->nbNotes[i], sizeof(Note));
    }

    curNotesOn = (int *)calloc(128, sizeof(int));

    for (staffIdx = 0; staffIdx < nbStaves; staffIdx++)
    {
        // Initialisation des notes actives
        for (i = 0; i < 128; i++)
        {
            curNotesOn[i] = -1;
        }

        trackIdx = staffToTrack[staffIdx];
        channelIdx = staffToChannel[staffIdx];

        noteIdx = 0;
        tempoIdx = 0;
        curTime = 0.0;
        for (i = 0; i < midi->nbNotes[trackIdx]; i++)
        {
            noteEvt = midi->notes[trackIdx] + i;
            if (noteEvt->channel == channelIdx)
            {
                // La note est sur le bon channel, on doit la traiter
                curBeat = midiTimeToBeat(noteEvt->apearingMidiTime, midi);

                // Recherche du tempo courant
                while ((tempoIdx + 1 < sheet->nbTempos) && (sheet->tempos[tempoIdx + 1].appearingBeat <= curBeat))
                {
                    tempoIdx++;
                }

                // calcul du temp réel
                curTime = sheet->tempos[tempoIdx].appearingTime;
                curTime += (curBeat - sheet->tempos[tempoIdx].appearingBeat) * (float)sheet->tempos[tempoIdx].msPerBeat / 1000000.0f;

                if (noteEvt->velocity > 0)
                {
                    // On traite une nouvelle note On
                    if (curNotesOn[noteEvt->note] != -1)
                    {
                        // La note précédente n'avait pas été éteinte
                        noteOnIdx = curNotesOn[noteEvt->note];
                        preBeat = sheet->notes[staffIdx][noteOnIdx].appearingBeat;
                        preTime = sheet->notes[staffIdx][noteOnIdx].appearingTime;

                        sheet->notes[staffIdx][noteOnIdx].durationBeat = curBeat - preBeat;
                        sheet->notes[staffIdx][noteOnIdx].durationTime = curTime - preTime;

                        curNotesOn[noteEvt->note] = -1;
                    }

                    curNotesOn[noteEvt->note] = noteIdx;

                    sheet->notes[staffIdx][noteIdx].value = noteEvt->note;
                    sheet->notes[staffIdx][noteIdx].appearingBeat = curBeat;
                    sheet->notes[staffIdx][noteIdx].appearingTime = curTime;

                    noteIdx++;
                }
                else
                {
                    // On traite une note Off

                    noteOnIdx = curNotesOn[noteEvt->note];
                    if (noteOnIdx != -1)
                    {
                        preBeat = sheet->notes[staffIdx][noteOnIdx].appearingBeat;
                        preTime = sheet->notes[staffIdx][noteOnIdx].appearingTime;

                        sheet->notes[staffIdx][noteOnIdx].durationBeat = curBeat - preBeat;
                        sheet->notes[staffIdx][noteOnIdx].durationTime = curTime - preTime;

                        curNotesOn[noteEvt->note] = -1;
                    }

                }
            }
        }
    }

    // Libération de la mémoire
    freeMidi(midi);

    free(curNotesOn);
    free(staffToTrack);
    free(staffToChannel);

    return sheet;
}

void freeSheetMusic(SheetMusic *sheet)
{
    int i;

    if (sheet)
    {
        if (sheet->nbNotes != NULL) free(sheet->nbNotes);
        if (sheet->tempos != NULL) free(sheet->tempos);
        if (sheet->notes != NULL)
        {
            for (i = 0; i < sheet->nbStaves; i++)
            {
                if (sheet->notes[i] != NULL) free(sheet->notes[i]);
            }
            free(sheet->notes);
        }
        memset(sheet, 0, sizeof(SheetMusic));
        free(sheet);
    }
}

void printNoteValue(Note *note)
{
    fprintNoteValue(stdout, note);
}

void fprintNoteValue(FILE *file, Note *note)
{
    int value = note->value;
    int height;

    if (!file || !note)
    {
        return;
    }

    if (value < 21)
    {
        fprintf(file, "??");
        return;
    }

    height = (value / 12) - 1;
    switch ((value - 21) % 12)
    {
    case 0:
        fprintf(file, "A_");
        break;
    case 1:
        fprintf(file, "A#");
        break;
    case 2:
        fprintf(file, "B_");
        break;
    case 3:
        fprintf(file, "C_");
        break;
    case 4:
        fprintf(file, "C#");
        break;
    case 5:
        fprintf(file, "D_");
        break;
    case 6:
        fprintf(file, "D#");
        break;
    case 7:
        fprintf(file, "E_");
        break;
    case 8:
        fprintf(file, "F_");
        break;
    case 9:
        fprintf(file, "F#");
        break;
    case 10:
        fprintf(file, "G_");
        break;
    default:
        fprintf(file, "G#");
        break;
    }
    fprintf(file, "%d", height);
}

void printStaff(SheetMusic *sheet, int staffIdx)
{
    fprintStaff(stdout, sheet, staffIdx);
}

void fprintStaff(FILE *file, SheetMusic *sheet, int staffIdx)
{
    int i, nbNotes;
    Note *note;

    if (!file || !sheet)
        return;
    if (staffIdx >= sheet->nbStaves)
        return;

    fprintf(file, "Staff %d, nbNotes %d\n", staffIdx, sheet->nbNotes[staffIdx]);
    fprintf(file, "  +--------+--------+------+-----------+\n");
    fprintf(file, "  |  Time  |  Beat  | Note | 0 1 2 3 4 |\n");
    fprintf(file, "  +--------+--------+------+-----------+\n");

    nbNotes = sheet->nbNotes[staffIdx];
    for (i = 0; i < nbNotes; i++)
    {
        note = &sheet->notes[staffIdx][i];
        fprintf(file, "  | %6.2f | %6.2f |  ", note->appearingTime, note->appearingBeat);
        fprintNoteValue(file, note);
        switch (note->stringIdx)
        {
        case 0:
            printf(" | 0         |\n");
            break;
        case 1:
            printf(" |   1       |\n");
            break;
        case 2:
            printf(" |     2     |\n");
            break;
        case 3:
            printf(" |       3   |\n");
            break;
        case 4:
            printf(" |         4 |\n");
            break;
        default:
            printf(" | ????????? |\n");
            break;
        }
    }
    fprintf(file, "  +--------+--------+------+-----------+\n");
}

void printSheetMusic(SheetMusic *sheet)
{
    fprintSheetMusic(stdout, sheet);
}

void fprintSheetMusic(FILE *file, SheetMusic *sheet)
{
    if (!file || !sheet)
        return;

    int i;
    fprintf(file, "nbStaves %d\n", sheet->nbStaves);
    for (i = 0; i < sheet->nbStaves; i++)
    {
        fprintStaff(file, sheet, i);
    }
}