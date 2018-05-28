
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "timer.h"
#include "model.h"

Timer * newTimer(float relSpeed)
{
    Timer *timer = NULL;

    timer = (Timer *)calloc(1, sizeof(Timer));
    if (!timer)
        return NULL;

    timer->relSpeed = relSpeed;
    timer->timeBeforeStrum = STRUM_AREA / relSpeed;
    timer->timeAfterStrum = (1.0f - STRUM_AREA) / relSpeed;
    timer->currentTime = -(timer->timeBeforeStrum);
    timer->previousTime = timer->currentTime;
    timer->delta = 0.0f;

    return timer;
}

void freeTimer(Timer * timer)
{
    free(timer);
}

void startTimer(Timer *timer)
{
    if (!timer)
        return;

    timer->startTime = SDL_GetTicks() / 1000.f;
    timer->currentTime = -(timer->timeBeforeStrum);
    timer->previousTime = timer->currentTime;
    timer->delta = 0.0f;
}

void updateTimer(Timer *timer)
{
    timer->previousTime = timer->currentTime;
    timer->currentTime = SDL_GetTicks() / 1000.f - timer->startTime - timer->timeBeforeStrum;
    timer->delta = timer->currentTime - timer->previousTime;
}