
#ifndef _MODEL_INCLUDED_
#define _MODEL_INCLUDED_

#define MAX_STRINGS 5
#define STRUM_AREA 0.85f
#define SPEED 0.13f

// Duree minimale de note pour la considerer longue
#define MINIMAL_LONG_NOTE 0.3

#include "../SheetMusic/analysis.h"
#include "../SheetMusic/sheetMusic.h"
#include "../GameControler/gameKeys.h"
#include "timer.h"

typedef enum GameNoteType_e
{
    typeShortNote,
    typeLongNote
} GameNoteType;

typedef enum GameNoteState_e
{
    stateAlive,  // En cours de descente
    statePlayed, // Jouee et validee
    stateHeld,   // Jouee et tenue
    stateDead,   // plus visible dans la fenêtre
    stateFailed  // Ratee
} GameNoteState;

typedef struct GameNote_s
{
    GameNoteType type;      // Type de note (court ou long)
    GameNoteState state;    // Etat de la note
    int value;     // valeur de la note
    int treated;   // Note traitee par la detection de frappe?
    int stringIdx; // Numero de corde
    int visible;   // Note visible ou invisible (affichage)
    float relPos;    // Position relative [0,1] de la note sur la corde
    float playingTime;   // Horaire d'apparition de la note
    float duration;      // Duree de la note
    float endingTime;    // Horaire de disparition de la note
    float ageAtDeath;    // Horaire de relachement precoce
} GameNote;

typedef struct GameSheet_s
{
    int nbStrings;  // nombre de cordes
    int nbStaves;   // Nombre d'instruments / portees / pistes
    int staffIdx;   // Portee en cours d'affichage
    int *nbNotes;   // Nombre de notes pour chaque instru
    GameNote **notes;   // Ensemble de Notes (nbTrack lignes de notes)
} GameSheet;

GameSheet * newGameSheet(SheetMusic * sheet);
void freeGameSheet(GameSheet * gameSheet);

void updateGameSheet(GameSheet *sheet, Timer *timer); // TODO Faites tomber les notes

typedef struct Model_s
{
    GameSheet * gameSheet;
    GameKeys * keys;
    Timer * timer;
    int points;
    int cOmbOMult;
    int xXcOmbOXx;
} Model;

Model * newModel(SheetMusic * sheet, float relSpeed);
void freeModel(Model * model);

void updateModel(Model *model);
void checkStrum(Model *model);

#endif
