#ifndef TIMER_H
#define TIMER_H



void timerInit(int valueMilliSec);


void timerDestroy();

void timerUpdateTimer(int valueMilliSec);


int getTimeDiff();


int timerTimeOver();


void timerPrint();

void timerWait(int ms);


#endif
