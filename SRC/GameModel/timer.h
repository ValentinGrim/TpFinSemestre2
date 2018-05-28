
#ifndef _TIMER_INCLUDED_
#define _TIMER_INCLUDED_

typedef struct Timer_s
{
    float startTime;
    float currentTime;
    float previousTime;
    float timeBeforeStrum;
    float timeAfterStrum;
    float delta;
    float relSpeed;
} Timer;

Timer * newTimer(float relSpeed);
void freeTimer(Timer * timer);

void startTimer(Timer * timer);
void updateTimer(Timer * timer);

#endif