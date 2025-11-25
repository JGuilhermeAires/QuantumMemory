#include <unistd.h>
#include "timer.h"

// Função exigida pelo main.c
void timerWait(int ms) {
    usleep(ms * 1000);  // milissegundos -> microssegundos
}
