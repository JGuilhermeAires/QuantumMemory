#include "screen.h"
#include <stdio.h>
#include <unistd.h>

// =====================
// Funções internas
// =====================
void screenClear()
{
    printf("%s[2J", ESC); // limpa a tela
    fflush(stdout);
}

void screenHomeCursor()
{
    printf("%s[H", ESC); // cursor para posição 0,0
}

void screenHideCursor()
{
    printf("%s[?25l", ESC); // esconde cursor
}

void screenShowCursor()
{
    printf("%s[?25h", ESC); // mostra cursor
}

void screenBoxEnable() { /* vazio */ }
void screenBoxDisable() { /* vazio */ }

void screenSetNormal()
{
    printf("%s[0m", ESC); // reset de cores
}

void screenUpdate()
{
    fflush(stdout); // força atualização da tela
}

// =====================
// Funções públicas
// =====================

void screenGotoxy(int x, int y)
{
    if (x < 0) x = 0;
    if (x > MAXX) x = MAXX;
    if (y < 0) y = 0;
    if (y > MAXY) y = MAXY;

    printf("%s[%d;%dH", ESC, y + 1, x + 1); // ANSI usa 1-based
}

void screenSetColor(screenColor fg, screenColor bg)
{
    int atr = 0; // normal
    if (fg > LIGHTGRAY)
    {
        atr = 1; // negrito
        fg -= 8;
    }
    printf("%s[%d;%d;%dm", ESC, atr, fg + 30, bg + 40);
}

void screenDrawBorders()
{
    screenClear();
    screenBoxEnable();

    char hbc = BOX_HLINE;
    char vbc = BOX_VLINE;

    // canto superior
    screenGotoxy(MINX, MINY);
    printf("%c", BOX_UPLEFT);
    for (int i = MINX + 1; i < MAXX; i++)
    {
        screenGotoxy(i, MINY);
        printf("%c", hbc);
    }
    screenGotoxy(MAXX, MINY);
    printf("%c", BOX_UPRIGHT);

    // laterais
    for (int i = MINY + 1; i < MAXY; i++)
    {
        screenGotoxy(MINX, i);
        printf("%c", vbc);
        screenGotoxy(MAXX, i);
        printf("%c", vbc);
    }

    // canto inferior
    screenGotoxy(MINX, MAXY);
    printf("%c", BOX_DWNLEFT);
    for (int i = MINX + 1; i < MAXX; i++)
    {
        screenGotoxy(i, MAXY);
        printf("%c", hbc);
    }
    screenGotoxy(MAXX, MAXY);
    printf("%c", BOX_DWNRIGHT);

    screenBoxDisable();
    screenUpdate();
}

void screenInit(int drawBorders)
{
    screenClear();
    if (drawBorders) screenDrawBorders();
    screenHomeCursor();
    screenHideCursor();
}

void screenDestroy()
{
    screenSetNormal();
    screenClear();
    screenHomeCursor();
    screenShowCursor();
}
