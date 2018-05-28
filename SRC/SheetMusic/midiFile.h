
#ifndef MIDI_FILE_H
#define MIDI_FILE_H

#include <stdio.h>

typedef unsigned char byte;
typedef unsigned long varlen;

typedef struct NoteEvt_s
{
    int apearingMidiTime;
    int channel;
    int note;
    int velocity;
} NoteEvt;

typedef struct SetTempoEvt_s
{
    int apearingMidiTime;
    int channel;
    int msPerBeat;
} SetTempoEvt;

typedef struct Midi_s
{
    int format;    // Format du fichier midi
    int division;  // Nombre de division d'un beat
    int nbTracks;  // Nombre de pistes

    int       *nbNotes;  // Nombre de notes ON par piste
    NoteEvt  **notes;    // Les notes ON

    int          nbTempos;  // Le nombre de changement de temp
    SetTempoEvt *tempos;    // les changements de tempo
} Midi;

Midi* newMidi(FILE *midiFile);
void freeMidi(Midi *midi);

int midiReadFileHeader(FILE *midiFile, Midi *midi);
int midiReadTrackHeader(FILE *midiFile, int *trackLen);
void midiReadTrack(const int trackIdx, byte *track, long trackLen, Midi *midi);

long midiReadLong(FILE *midiFile);
short midiReadShort(FILE *midiFile);
varlen midiReadVarLen(FILE *midiFile);

typedef enum MidiCommand_e
{
    // Channel voice messages
    noteOff = 0x80,
    noteOn = 0x90,
    polyphonicKeyPressure = 0xA0,
    controlChange = 0xB0,
    programChange = 0xC0,
    channelPressure = 0xD0,
    pitchBend = 0xE0,

    // Channel mode messages
    channelMode = 0xB8,

    // System messages
    systemExclusive = 0xF0,
    systemCommon = 0xF0,
    systemExclusivePacket = 0xF7,
    systemRealTime = 0xF8,
    systemStartCurrentSequence = 0xFA,
    systemContinueCurrentSequence = 0xFB,
    systemStop = 0xFC,

    // MIDI file-only messages
    fileMetaEvent = 0xFF
} MidiCommand;


typedef enum MidiMetaEvent_e
{
    // MIDI file meta-event codes
    sequenceNumberMetaEvent = 0,
    textMetaEvent = 1,
    copyrightMetaEvent = 2,
    trackTitleMetaEvent = 3,
    trackInstrumentNameMetaEvent = 4,
    lyricMetaEvent = 5,
    markerMetaEvent = 6,
    cuePointMetaEvent = 7,

    channelPrefixMetaEvent = 0x20,
    portMetaEvent = 0x21,
    endTrackMetaEvent = 0x2F,

    setTempoMetaEvent = 0x51,
    smpteOffsetMetaEvent = 0x54,
    timeSignatureMetaEvent = 0x58,
    keySignatureMetaEvent = 0x59,

    sequencerSpecificMetaEvent = 0x7F
} MidiMetaEvent;

#endif