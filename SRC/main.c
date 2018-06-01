
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifdef _WIN32
#include <SDL.h>
#include <SDL_mixer.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

#include "GameModel/model.h"
#include "GameDisplay/display.h"
#include "GameDisplay/mainWindow.h"
#include "GameDisplay/textures.h"
#include "GameControler/gameKeys.h"

int playMusicWithDelay(void *ptr)
{
    MusicThread *music = (MusicThread*)ptr;
    int delay = (int)(music->t * 1000);
    printf("Thread Delay = %d\n", delay);
#ifdef _WIN32
    Sleep(delay);
#else
    usleep(delay * 1000);
#endif

    Mix_PlayMusic(music->music, 1);
    return 0;
}

int main(int argc, char** argv)
{

    char** new_argv = malloc((argc+1) * sizeof *new_argv);
    for(int i = 0; i < argc; ++i)
    {
        size_t length = strlen(argv[i])+1;
        new_argv[i] = malloc(length);
        memcpy(new_argv[i], argv[i], length);
    }
    new_argv[argc] = NULL;

    MainWindow * mainWindow = NULL;
    SDLGameConfig * config = NULL;
    Mix_Music * music = NULL;
    MusicThread musicThread;
    Model * model = NULL;
    GameDisplay  * gameDisp = NULL;
    SheetMusic * sheet = NULL;
    FILE  *midiFile = NULL;
    int nbStrings;
    int pianoMode = 1;
    int staffIdx;
    float relSpeed;

    //******************************************************************************************************************
    // R�cup�ration des arguments du main

    if (argc < 3)
    {
        printf("Arguments insuffisants\n");
        return EXIT_FAILURE;
    }

    midiFile = fopen(argv[1], "rb");
    if (!midiFile)
    {
        printf("Erreur d'ouverture du fichier %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    nbStrings = atoi(argv[2]);
    staffIdx = (argc > 3) ? atoi(argv[3]) : 0;

    //******************************************************************************************************************
    // Traitement du midi et de la partition

    // Lecture du fichier midi
    sheet = newSheetMusic(midiFile);
    if (!sheet)
    {
        printf("Erreur de creation de la partition\n");
        return EXIT_FAILURE;
    }
    fclose(midiFile);

    // Traitement de la partition
    sheet->nbStrings = nbStrings;
    placeNotes(sheet);

    // Affichage de la partition
    printSheetMusic(sheet);

    //******************************************************************************************************************
    // Initialisation du mod�le

    relSpeed = nbStrings * SPEED;
    model = newModel(sheet, relSpeed, new_argv[1]);
    if (!model)
    {
        printf("Erreur de creation du modele\n");
        return EXIT_FAILURE;
    }
    freeSheetMusic(sheet);

    model->gameSheet->staffIdx = staffIdx;

    //******************************************************************************************************************
    // Initialisation de la vue

    initSDL();
    mainWindow = newMainWindow();
    if (!mainWindow)
    {
        printf("Erreur de creation de la fenetre\n");
        return EXIT_FAILURE;
    }
    gameDisp = newGameDisplay(mainWindow);
    if (!gameDisp)
    {
        printf("Erreur de creation de l'affichage\n");
        return EXIT_FAILURE;
    }
    initMetrics(gameDisp->metrics, nbStrings);

    //******************************************************************************************************************
    // Initialisation du controller

    config = newGameConfig(pianoMode);
    if (!config)
    {
        printf("Erreur de creation du controller\n");
        return EXIT_FAILURE;
    }

    //******************************************************************************************************************
    // Initialisation de la musique

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
    {
        printf("Erreur de chargement du mixer : %s\n", Mix_GetError());
        return EXIT_FAILURE;
    }
    music = Mix_LoadMUS(argv[1]);
    if (music == NULL)
    {
        printf("Erreur de chargement de la musique : %s\n", argv[1]);
        printf("  SDL : %s\n", Mix_GetError());
        return EXIT_FAILURE;
    }
#ifdef _WIN32
    musicThread.t = STRUM_AREA / relSpeed - 0.30f;
#else
    musicThread.t = STRUM_AREA / relSpeed;
#endif
    musicThread.music = music;
    SDL_CreateThread(playMusicWithDelay, "music", &musicThread);

    //******************************************************************************************************************
    // Boucle de rendu

    // Lancement du timer
    startTimer(model->timer);

   	model->Menu = 0;
    model->keys->Startgame = 0;
    model->choixMenu = 0;
    while (model->Menu != -1)     // Menu == -1   =>   quitter le jeu
    {
        if(model->Menu != 1)
            model->keys->Startgame = 0;
        // Mise à jour du controller
        processGameEvents(config, model->keys);
        if(model->keys->Startgame > 1)
            model->choixMenu = model->keys->Startgame - 2;
        // Mise à jour du modèle
        updateModel(model);


        // Mise à jour de la vue
        updateGameDisplay(gameDisp, mainWindow, model);
        if(model->keys->Startgame == 1)
            model->Menu = 1;
        if(model->keys->quitDown || model->keys->exitDown)
        {
            if(model->Menu == 0) //le Menu est ouvert
                model->Menu = -1; // on ferme la fenètre
            else if(model->Menu == 1) // on est dans le jeu
                model->Menu = 0;  // on retourne au Menu
        }
    }

    //******************************************************************************************************************
    // Lib�ration de la m�moire

    freeGameConfig(config);
    freeModel(model);
    freeGameDisplay(gameDisp);
    freeMainWindow(mainWindow);

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    quitSDL();

    for(int i = 0; i < argc; ++i)
    {
      free(new_argv[i]);
    }
    free(new_argv);

    return EXIT_SUCCESS;
}
