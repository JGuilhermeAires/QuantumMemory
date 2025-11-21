#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Bibliotecas/screen.h"
#include "Bibliotecas/keyboard.h"
#include "Bibliotecas/timer.h"

// ===================================================
// CONFIGURAÇÕES
// ===================================================
#define MAXCARTAS 16
#define MAXNOME   50

typedef enum { FACIL=1, NORMAL=2, DIFICIL=3 } Dificuldade;

Dificuldade dificuldadeSelecionada = NORMAL;
int tempoShuffle = 600;      
int tempoAnimacao = 100;     

// ===================================================
// ESTRUTURAS
// ===================================================
typedef struct { int id; int estado; } Carta;
typedef struct { char nome[MAXNOME]; int score; } Registro;

// ===================================================
// VARIÁVEIS GLOBAIS
// ===================================================
Carta cartas[MAXCARTAS];
Registro rankingList[200];
char playerName[MAXNOME];

int totalReg = 0;
int selecionada = 0;
int primeira = -1;
int segunda = -1;
int score = 0;
int paresRestantes = MAXCARTAS / 2;

// ===================================================
// SOMS
// ===================================================
void beepOk()   { printf("\a"); fflush(stdout); }
void beepErro() { printf("\a\a"); fflush(stdout); }

// ===================================================
// RANKING
// ===================================================
void salvarRanking() {
    FILE* f = fopen("ranking.dat", "a");
    if (!f) return;
    fprintf(f, "%s %d\n", playerName, score);
    fclose(f);
}

void carregarRanking() {
    totalReg = 0;
    FILE* f = fopen("ranking.dat", "r");
    if (!f) return;
    while (fscanf(f, "%s %d", rankingList[totalReg].nome, &rankingList[totalReg].score) != EOF)
        totalReg++;
    fclose(f);
}

void ordenarRanking() {
    for (int i = 0; i < totalReg; i++)
        for (int j = i+1; j < totalReg; j++)
            if (rankingList[j].score > rankingList[i].score) {
                Registro temp = rankingList[i];
                rankingList[i] = rankingList[j];
                rankingList[j] = temp;
            }
}

int posicaoJogador() {
    for (int i = 0; i < totalReg; i++)
        if (strcmp(rankingList[i].nome, playerName)==0 && rankingList[i].score==score)
            return i+1;
    return -1;
}

// ===================================================
// ANIMAÇÃO
// ===================================================
void animacaoShuffle() {
    for(int i=0; i<3; i++) {
        screenGotoxy(2,24);
        screenSetColor(RED, BLACK);
        printf("*** COLAPSO QUANTICO ***");
        screenUpdate();
        timerWait(tempoAnimacao);

        screenGotoxy(2,24);
        screenSetColor(BLACK, BLACK);
        printf("                       ");
        screenUpdate();
        timerWait(tempoAnimacao);
    }
}

// ===================================================
// EMBARALHAR
// ===================================================
void quantumShuffle() {
    animacaoShuffle();
    int a = rand()%MAXCARTAS;
    int b = rand()%MAXCARTAS;
    if (cartas[a].estado!=2 && cartas[b].estado!=2) {
        Carta temp = cartas[a];
        cartas[a] = cartas[b];
        cartas[b] = temp;
    }
}

// ===================================================
// INICIALIZA CARTAS
// ===================================================
void initCartas() {
    int ids[MAXCARTAS];
    for(int i=0;i<MAXCARTAS;i++) ids[i] = i/2;
    for(int i=0;i<MAXCARTAS;i++) {
        int r = rand()%MAXCARTAS;
        int t = ids[i]; ids[i]=ids[r]; ids[r]=t;
    }
    for(int i=0;i<MAXCARTAS;i++) { cartas[i].id=ids[i]; cartas[i].estado=0; }
    primeira = segunda = -1;
    score = 0;
    paresRestantes = MAXCARTAS/2;
}

// ===================================================
// DESENHAR TABULEIRO
// ===================================================
void drawBoard() {
    screenClear();
    screenSetColor(CYAN, BLACK);
    screenGotoxy(2,1);
    printf("QuantumMemory - Jogador: %s | Pontos: %d | Restantes: %d",
           playerName, score, paresRestantes);

    int x=6,y=4;
    for(int i=0;i<MAXCARTAS;i++) {
        screenGotoxy(x,y);
        if(i==selecionada) screenSetColor(YELLOW, BLUE);
        else screenSetColor(WHITE, BLACK);

        if(cartas[i].estado==2) printf("[★]");
        else if(cartas[i].estado==1) printf(" %d ", cartas[i].id);
        else printf("[■]");

        x+=6;
        if((i+1)%4==0){ y+=2; x=6; }
    }
    screenUpdate();
}

// ===================================================
// ABRIR CARTA
// ===================================================
void abrirCarta(int i) {
    if(cartas[i].estado!=0) return;

    cartas[i].estado=1;
    drawBoard();
    timerWait(120);

    if(primeira==-1){ primeira=i; return; }

    segunda=i;
    if(cartas[primeira].id==cartas[segunda].id) {
        cartas[primeira].estado=2;
        cartas[segunda].estado=2;
        beepOk();
        score++;
        paresRestantes--;
    } else {
        beepErro();
        cartas[primeira].estado=0;
        cartas[segunda].estado=0;
    }
    primeira=segunda=-1;
}

// ===================================================
// PEDIR NOME
// ===================================================
void pedirNome() {
    screenClear();
    screenGotoxy(10,10);
    printf("Digite seu nome: ");
    screenUpdate();

    fgets(playerName, MAXNOME, stdin);
    // Remove newline character
    size_t len = strlen(playerName);
    if (len > 0 && playerName[len-1] == '\n')
        playerName[len-1] = '\0';
}

// ===================================================
// MOSTRAR RANKING
// ===================================================
void mostrarRanking() {
    carregarRanking();
    ordenarRanking();

    screenClear();
    screenGotoxy(5,2);
    screenSetColor(YELLOW, BLACK);
    printf("RANKING GERAL");

    for(int i=0;i<totalReg && i<20;i++) {
        screenGotoxy(5,4+i);
        printf("%d - %-20s %d", i+1, rankingList[i].nome, rankingList[i].score);
    }

    int pos = posicaoJogador();
    screenGotoxy(5,27);
    if(pos>0) printf("Voce ficou na posicao %d!", pos);
    else printf("Posicao nao identificada.");

    screenUpdate();
    getchar(); getchar(); // pausa para o jogador
}

// ===================================================
// MENU DE DIFICULDADE
// ===================================================
void menuDificuldade() {
    screenClear();
    screenSetColor(WHITE, BLACK);
    screenGotoxy(10,9);
    printf("Selecione a dificuldade:");
    screenGotoxy(10,11); printf("1 - Facil");
    screenGotoxy(10,12); printf("2 - Normal");
    screenGotoxy(10,13); printf("3 - Dificil");
    screenUpdate();

    int op;
    scanf("%d",&op);
    // Limpa o buffer após scanf
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    switch(op){
        case 1: dificuldadeSelecionada=FACIL; tempoShuffle=1000; break;
        case 2: dificuldadeSelecionada=NORMAL; tempoShuffle=600; break;
        case 3: dificuldadeSelecionada=DIFICIL; tempoShuffle=300; break;
        default: dificuldadeSelecionada=NORMAL; break;
    }
}

// ===================================================
// LOOP DO JOGO
// ===================================================
void jogar() {
    initCartas();
    drawBoard();
    timerInit(tempoShuffle);

    while(paresRestantes>0) {
        if(timerTimeOver()) {
            quantumShuffle();
            drawBoard();
        }

        if(keyhit()) {
            int c = readch();
            if(c==27) break;

            if(c=='w'||c=='W'){ if(selecionada>=4) selecionada-=4; }
            if(c=='s'||c=='S'){ if(selecionada<12) selecionada+=4; }
            if(c=='a'||c=='A'){ if(selecionada%4!=0) selecionada--; }
            if(c=='d'||c=='D'){ if(selecionada%4!=3) selecionada++; }

            if(c==10||c==13) abrirCarta(selecionada);

            drawBoard();
        }
    }

    salvarRanking();
    timerDestroy();
}

// ===================================================
// MENU PRINCIPAL
// ===================================================
int menu() {
    screenClear();
    screenGotoxy(10,10);
    screenSetColor(WHITE, BLACK);
    printf("QUANTUM MEMORY");
    screenGotoxy(10,12); printf("1 - Jogar");
    screenGotoxy(10,13); printf("2 - Ranking");
    screenGotoxy(10,14); printf("3 - Dificuldade");
    screenGotoxy(10,15); printf("4 - Sair");
    screenUpdate();

    int op;
    scanf("%d",&op);
    // Limpa o buffer após scanf
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return op;
}

// ===================================================
// MAIN
// ===================================================
int main() {
    srand(time(NULL));
    screenInit(1);
    keyboardInit();

    int op=0;
    while(op!=4){
        op=menu();
        switch(op){
            case 1: pedirNome(); jogar(); mostrarRanking(); break;
            case 2: mostrarRanking(); break;
            case 3: menuDificuldade(); break;
            default: break;
        }
    }

    keyboardDestroy();
    screenDestroy();
    return 0;
}
    