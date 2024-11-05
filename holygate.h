#ifndef HOLYGATE_H
#define HOLYGATE_H

#include "raylib.h" // Para usar o tipo Texture2D

// Definição da struct Magia
typedef struct Magia {
    char nome[50];         // Nome da magia
    int tipo;              // Tipo da magia (ex: 1 = Fogo, 2 = Gelo, etc.)
    float velocidade;
    Color cor;     // Textura da magia
} Magia;

typedef struct MagiaAtiva {
    Magia magia;
    Vector2 posicao;
    struct MagiaAtiva *proxima;
} MagiaAtiva;

// Definição da struct Monstro
typedef struct Monstro {
    char nome[50];         // Nome do monstro
    int vida;              // Vida do monstro
    float velocidade;      // Velocidade do monstro
    Vector2 posicao;       // Posição do monstro
    struct Magia fraqueza; // Fraqueza do monstro
    Color cor;             // Cor do monstro
} Monstro;

typedef struct Node {
    Monstro monstro;
    struct Node *proximo;
    struct Node *anterior;
} Node;

typedef struct Padre {
    int vida;
    Vector2 posicao;
    Color cor;
} Padre;


#endif