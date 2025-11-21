#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>

// =========================
//   CONSTANTES DA TELA
// =========================
#define MINX  0
#define MINY  0
#define MAXX  79
#define MAXY  23

// =========================
//   SEQUÊNCIA ESC ANSI
// =========================
#define ESC "\x1b"

// =========================
//   CARACTERES DE CAIXA
// =========================
#define BOX_HLINE     196   // '─'
#define BOX_VLINE     179   // '│'
#define BOX_UPLEFT    218   // '┌'
#define BOX_UPRIGHT   191   // '┐'
#define BOX_DWNLEFT   192   // '└'
#define BOX_DWNRIGHT  217   // '┘'

// =========================
//   ENUM DE CORES
// =========================
typedef enum
{
BLACK = 0,
BLUE,
GREEN,
CYAN,
RED,
MAGENTA,
BROWN,
LIGHTGRAY,


DARKGRAY,
LIGHTBLUE,
LIGHTGREEN,
LIGHTCYAN,
LIGHTRED,
LIGHTMAGENTA,
YELLOW,
WHITE


} screenColor;

// =========================
//   FUNÇÕES
// =========================

// Inicializa a tela (limpa e pode desenhar borda)
void screenInit(int drawBorders);

// Restaura terminal e mostra cursor novamente
void screenDestroy();

// Move cursor sem quebrar a tela
void screenGotoxy(int x, int y);

// Define cor de texto e fundo
void screenSetColor(screenColor fg, screenColor bg);

// Desenha bordas completas
void screenDrawBorders();

// -------------------------
// Funções auxiliares que seu .c usa internamente
// (Devem existir em outro .c)
// -------------------------

void screenClear();
void screenHomeCursor();
void screenHideCursor();
void screenShowCursor();
void screenBoxEnable();
void screenBoxDisable();
void screenSetNormal();

#endif
