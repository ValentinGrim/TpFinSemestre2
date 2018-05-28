
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "midiFile.h"

varlen vlength(byte **track, long *trackLen)
{
    varlen value;
    byte ch;
    byte *cp = *track;

    trackLen--;
    if ((value = *cp++) & 0x80)
    {
        value &= 0x7F;
        do
        {
            value = (value << 7) | ((ch = *cp++) & 0x7F);
            trackLen--;
        } while (ch & 0x80);
    }
    *track = cp;
    return value;
}

long midiReadLong(FILE *midiFile)
{
    unsigned char c[4];

    fread((char *)c, 1, sizeof c, midiFile);
    return (long)((c[0] << 24) | (c[1] << 16) | (c[2] << 8) | c[3]);
}

short midiReadShort(FILE *midiFile)
{
    unsigned char c[2];

    fread((char *)c, 1, sizeof c, midiFile);
    return (short)((c[0] << 8) | c[1]);
}

varlen midiReadVarLen(FILE *midiFile)
{
    long value;
    int ch;

    if ((value = getc(midiFile)) & 0x80)
    {
        value &= 0x7F;
        do
        {
            value = (value << 7) | ((ch = getc(midiFile)) & 0x7F);
        } while (ch & 0x80);
    }
    return value;
}

int midiReadFileHeader(FILE *midiFile, Midi *midi)
{
    char chunkType[4];
    long headerLength;

    fread(chunkType, sizeof(char), 4, midiFile);

    if (memcmp(chunkType, "MThd", 4 * sizeof(char)) != 0)
    {
        fprintf(stderr, "Not a Standard MIDI File.\n");
        return 2;
    }

    headerLength = midiReadLong(midiFile); // Vaut toujours 6
    midi->format = midiReadShort(midiFile);
    midi->nbTracks = midiReadShort(midiFile);
    midi->division = midiReadShort(midiFile);

    return EXIT_SUCCESS;
}

int midiReadTrackHeader(FILE *midiFile, int *trackLen)
{
    char chunkType[4];

    fread(chunkType, sizeof(char), 4, midiFile);
    if (memcmp(chunkType, "MTrk", 4 * sizeof(char)) != 0)
    {
        fprintf(stderr, "Track header is invalid.\n");
        return 2;
    }
    (*trackLen) = midiReadLong(midiFile);

    return EXIT_SUCCESS;
}

Midi* newMidi(FILE *midiFile)
{
    Midi *midi = NULL;
    int i, trackIdx;
    int nbTracks;
    int trackLen;
    int maxNbEvt;
    int totalNbNotes;
    int   *tracksLen; // Longueurs des pistes
    byte **tracks;    // Pistes

    if (!midiFile)
        return NULL;
    rewind(midiFile);

    midi = (Midi *)calloc(1, sizeof(Midi));

    // Lecture du header du fichier midi
    midiReadFileHeader(midiFile, midi);

    // Allocation des longueurs des pistes
    nbTracks = midi->nbTracks;
    tracksLen = (int *)calloc(nbTracks, sizeof(int));

    // Allocation des pistes
    tracks = (byte **)calloc(nbTracks, sizeof(byte *));

    //******************************************************************************************************************
    // Lecture des headers et des pistes

    for (i = 0; i < nbTracks; i++)
    {
        /* Lecture du header de la piste */
        midiReadTrackHeader(midiFile, &tracksLen[i]);

        /* Allocation de la piste */
        trackLen = tracksLen[i];
        tracks[i] = (byte *)calloc(trackLen, sizeof(byte));

        /* Lecture de la piste */
        fread(tracks[i], sizeof(byte), trackLen, midiFile);
    }

    //******************************************************************************************************************
    // Analyse des événements midi

    maxNbEvt = 0;
    for (i = 0; i < nbTracks; i++)
    {
        maxNbEvt += tracksLen[i];
    }

    // Allocation des évenements

    midi->nbNotes = (int *)calloc(nbTracks, sizeof(int));
    midi->notes = (NoteEvt **)calloc(nbTracks, sizeof(NoteEvt *));

    midi->notes[0] = (NoteEvt *)calloc(maxNbEvt, sizeof(NoteEvt));
    midi->tempos = (SetTempoEvt *)calloc(maxNbEvt, sizeof(SetTempoEvt));

    totalNbNotes = 0;
    for (trackIdx = 0; trackIdx < nbTracks; trackIdx++)
    {
        // Initialisation du pointeur sur les notes
        midi->notes[trackIdx] = midi->notes[0] + totalNbNotes;

        // Lecture de la piste Midi
        midiReadTrack(trackIdx, tracks[trackIdx], tracksLen[trackIdx], midi);

        totalNbNotes += midi->nbNotes[trackIdx];
    }

    //******************************************************************************************************************
    // Libération de la mémoire

    for (i = 0; i < nbTracks; i++)
    {
        free(tracks[i]);
    }
    free(tracks);
    free(tracksLen);

    return midi;
}

void midiReadTrack(const int trackIdx, byte *track, long trackLen, Midi *midi)
{
    int levt = 0, evt, channel, note, velocity, control, value, type, msPerBeat;
    varlen length;
    byte *trackItem;
    varlen absMidiTime = 0; // Absolute time in track

    int noteIdx, tempoIdx;

    tempoIdx = midi->nbTempos;
    noteIdx = 0;

    // Allocation

    while (trackLen > 0)
    {
        varlen tlapse = vlength(&track, &trackLen);
        absMidiTime += tlapse;

        // Handle running status; if the next byte is a data byte, reuse the last command seen in the track.

        if (*track & 0x80)
        {
            evt = *track++;

            if ((evt & 0xF0) != 0xF0)
            {
                levt = evt;
            }
            trackLen--;
        }
        else
        {
            evt = levt;
        }

        channel = evt & 0xF;

        //**************************************************************************************************************
        // Channel messages

        switch (evt & 0xF0)
        {

        case noteOff: //....................................................................................... Note off
            if (trackLen < 2)
            {
                return;
            }
            trackLen -= 2;
            note = *track++;
            velocity = *track++;

            midi->notes[trackIdx][noteIdx].apearingMidiTime = absMidiTime;
            midi->notes[trackIdx][noteIdx].channel = channel;
            midi->notes[trackIdx][noteIdx].note = note;
            midi->notes[trackIdx][noteIdx].velocity = 0;
            noteIdx++;
            continue;

        case noteOn: //......................................................................................... Note on
            if (trackLen < 2)
            {
                return;
            }
            trackLen -= 2;
            note = *track++;
            velocity = *track++;

            midi->notes[trackIdx][noteIdx].apearingMidiTime = absMidiTime;
            midi->notes[trackIdx][noteIdx].channel = channel;
            midi->notes[trackIdx][noteIdx].note = note;
            midi->notes[trackIdx][noteIdx].velocity = velocity;
            noteIdx++;
            continue;

        case polyphonicKeyPressure: //....................................................................... Aftertouch
            if (trackLen < 2)
            {
                return;
            }
            trackLen -= 2;
            note = *track++;
            velocity = *track++;
            continue;

        case controlChange: //........................................................................... Control change
            if (trackLen < 2)
            {
                return;
            }
            trackLen -= 2;
            control = *track++;
            value = *track++;
            continue;

        case programChange: //........................................................................... Program change
            if (trackLen < 1)
            {
                return;
            }
            trackLen--;
            note = *track++;
            continue;

        case channelPressure: //.......................................................... Channel pressure (aftertouch)
            if (trackLen < 1)
            {
                return;
            }
            trackLen--;
            velocity = *track++;
            continue;

        case pitchBend: //................................................................................... Pitch bend
            if (trackLen < 1)
            {
                return;
            }
            trackLen--;
            value = *track++;
            value = value | ((*track++) << 7);
            continue;

        default: //............................................................................... Unkonwn channel event
            break;
        }

        switch (evt)
        {

            //**********************************************************************************************************
            // System exclusive messages

        case systemExclusive:
        case systemExclusivePacket:
            length = vlength(&track, &trackLen);
            track += length;
            trackLen -= length;
            break;

            //**********************************************************************************************************
            // File meta-events

        case fileMetaEvent:

            if (trackLen < 2)
            {
                return;
            }
            trackLen -= 2;
            type = *track++;
            length = vlength(&track, &trackLen);
            trackItem = track;
            track += length;
            trackLen -= length;

            switch (type)
            {
            case endTrackMetaEvent: //................................................................. End of the track
                trackLen = -1;
                break;

            case setTempoMetaEvent: //........................................................................ Set tempo
                msPerBeat = (trackItem[0] << 16) | (trackItem[1] << 8) | trackItem[2];
                midi->tempos[tempoIdx].apearingMidiTime = absMidiTime;
                midi->tempos[tempoIdx].channel = channel;
                midi->tempos[tempoIdx].msPerBeat = msPerBeat;
                tempoIdx++;
                break;

            case timeSignatureMetaEvent: //.............................................................. Time signature
                //printf("  Beat : %d, Channel : %d, Time_signature, %d, %d, %d, %d\n",
                //    absMidiTime, channel,
                //    trackItem[0], trackItem[1], trackItem[2], trackItem[3]);
                break;

            case sequencerSpecificMetaEvent:
            case sequenceNumberMetaEvent:
            case textMetaEvent:
            case copyrightMetaEvent:
            case trackTitleMetaEvent:
            case trackInstrumentNameMetaEvent:
            case lyricMetaEvent:
            case markerMetaEvent:
            case cuePointMetaEvent:
            case channelPrefixMetaEvent:
            case portMetaEvent:
            case smpteOffsetMetaEvent:
            case keySignatureMetaEvent:
                break;

            default: //.............................................................................. Unknown meta event
                break;
            }
            break;


            //**********************************************************************************************************
            // Unknown events

        default: //....................................................................................... Unknown event
            break;
        }
    }

    midi->nbNotes[trackIdx] = noteIdx;
    midi->nbTempos = tempoIdx;
}

void freeMidi(Midi *midi)
{
    if (midi)
    {
        if (midi->nbNotes != NULL) free(midi->nbNotes);
        if (midi->notes != NULL)
        {
            if (midi->notes[0] != NULL) free(midi->notes[0]);
            free(midi->notes);
        }
        if (midi->tempos != NULL) free(midi->tempos);
        free(midi);
    }
}