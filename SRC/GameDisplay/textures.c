
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textures.h"
#include "mainWindow.h"

Textures * newTextures(SDL_Renderer *renderer)
{
    Textures * textures = NULL;
    char imgPath[1024] = "../Images/";
    char *posToWrite = imgPath + 10;
    int i;

    if (!renderer)
        return NULL;

    textures = (Textures *)calloc(1, sizeof(Textures));

    for (i = 0; i < MAX_STRINGS; i++)
    {
        // Chargement de la note
        sprintf(posToWrite, "note_%d.png", i + 1);
        textures->notes[i] = loadTexture(imgPath, renderer);
        if (textures->notes[i] == NULL) goto ERROR_LABEL;
    }

    // Chargement du fond de la zone de jeu
    sprintf(posToWrite, "game_area.png");
    textures->gameArea = loadTexture(imgPath, renderer);
    if (textures->gameArea == NULL) goto ERROR_LABEL;

    return textures;

ERROR_LABEL:
    freeTextures(textures);
    return NULL;
}


void freeTextures(Textures *textures)
{
    int i;

    if (textures)
    {
        for (i = 0; i < MAX_STRINGS; i++)
        {
            // Libération de la note
            if (textures->notes[i])
                destroyTexture(textures->notes[i]);
        }

        // Libération du fond de la zone de jeu
        if (textures->gameArea)
            destroyTexture(textures->gameArea);

        free(textures);
    }
}

SDL_Texture * loadTexture(char *imgPath, SDL_Renderer *renderer)
{
    SDL_Texture *texture = NULL;

    texture = IMG_LoadTexture(renderer, imgPath);
    if (!texture)
    {
        fprintf(stderr, "Erreur de chargement de la texture : %s\n", imgPath);
        fprintf(stderr, "  SDL : %s\n", SDL_GetError());
    }

    return texture;
}

void destroyTexture(SDL_Texture * texture)
{
    SDL_DestroyTexture(texture);
}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;

    // On réccupère la largeur et la hauteur de la texture
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

    // On la copie dans le rendu
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

