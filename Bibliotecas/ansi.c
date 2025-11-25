#include <stdio.h>
#include "screen.h"

void screenUpdate() {
    fflush(stdout);   // força atualização da saída
}

void screenClear() {
    printf("%s[2J", ESC);
}

void screenHomeCursor() {
    printf("%s[H", ESC);
}

void screenHideCursor() {
    printf("%s[?25l", ESC);
}

void screenShowCursor() {
    printf("%s[?25h", ESC);
}

void screenSetNormal() {
    printf("%s[0m", ESC);
}

void screenBoxEnable() {}

void screenBoxDisable() {}
