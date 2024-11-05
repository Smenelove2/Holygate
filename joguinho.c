#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <time.h>

#define MAX_MONSTROS 100 // Limite de monstros na tela ao mesmo tempo

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

typedef struct Padre {
    int vida;
    Vector2 posicao;
    Color cor;
} Padre;

typedef struct Monstro {
    char nome[50];
    int vida;
    Vector2 posicao;
    Color cor;
    float velocidade; // Adiciona a velocidade do monstro
} Monstro;

typedef struct Node {
    Monstro monstro;
    struct Node *proximo;
    struct Node *anterior;
} Node;

// Função para criar um novo nó
Node* criarNode(Monstro monstro) {
    Node* novoNode = (Node*)malloc(sizeof(Node));
    if (novoNode == NULL) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }
    novoNode->monstro = monstro;
    novoNode->proximo = NULL;
    novoNode->anterior = NULL;
    return novoNode;
}

// Função para inserir um nó no final da lista duplamente encadeada
void inserirNode(Node** head, Monstro monstro) {
    Node* novoNode = criarNode(monstro);
    if (*head == NULL) {
        *head = novoNode;
    } else {
        Node* temp = *head;
        while (temp->proximo != NULL) {
            temp = temp->proximo;
        }
        temp->proximo = novoNode;
        novoNode->anterior = temp;
    }
}

// Função para carregar monstros de um arquivo .txt
void carregarMonstros(const char *filename, Node** head) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Monstro monstro;
    int r, g, b, a;

    // Lê o arquivo linha por linha e armazena os dados em cada monstro
    while (fscanf(file, "%49s %d %f %f %d %d %d %d %f", 
                  monstro.nome, 
                  &monstro.vida, 
                  &monstro.posicao.x, 
                  &monstro.posicao.y, 
                  &r, &g, &b, &a, 
                  &monstro.velocidade) == 9) 
    {
        monstro.cor = (Color){r, g, b, a};
        inserirNode(head, monstro);
    }

    fclose(file);
}

// Função para liberar memória da lista duplamente encadeada
void liberarLista(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->proximo;
        free(temp);
    }
}

// Função para escolher um monstro aleatório da lista
Node* escolherMonstroAleatorio(Node* head) {
    Node* temp = head;
    int count = 0;

    // Contar quantos monstros existem na lista
    while (temp != NULL) {
        count++;
        temp = temp->proximo;
    }

    if (count == 0) return NULL;

    // Seleciona um índice aleatório
    int indiceAleatorio = rand() % count;

    // Acessa o monstro no índice aleatório
    temp = head;
    for (int i = 0; i < indiceAleatorio; i++) {
        temp = temp->proximo;
    }

    return temp;
}

// Estrutura para controlar cada monstro em movimento
typedef struct {
    Node* node;
    Vector2 posicao;
    float velocidade; // Armazena a velocidade do monstro
    bool ativo;
} MonstroMovendo;

// Função para inicializar o array de monstros em movimento
void inicializarMonstrosMovendo(MonstroMovendo monstros[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        monstros[i].node = NULL;
        monstros[i].ativo = false;
        monstros[i].velocidade = 0.0f;
    }
}

// Função para ativar um novo monstro em movimento
void ativarMonstro(MonstroMovendo monstros[], int tamanho, Node* listaMonstros, int larguraTela, int alturaTela) {
    for (int i = 0; i < tamanho; i++) {
        if (!monstros[i].ativo) {
            Node* monstroEscolhido = escolherMonstroAleatorio(listaMonstros);
            if (monstroEscolhido != NULL) {
                monstros[i].node = monstroEscolhido;
                monstros[i].posicao = (Vector2){ larguraTela - 50, alturaTela - 70 }; // Posição inicial na direita inferior
                monstros[i].velocidade = monstroEscolhido->monstro.velocidade; // Define a velocidade do monstro
                monstros[i].ativo = true;
            }
            break;
        }
    }
}

// Função para atualizar a posição dos monstros em movimento
void atualizarMonstrosMovendo(MonstroMovendo monstros[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (monstros[i].ativo) {
            monstros[i].posicao.x -= monstros[i].velocidade; // Usa a velocidade específica de cada monstro

            // Desativa o monstro se ele sair da tela
            if (monstros[i].posicao.x < -50) {
                monstros[i].ativo = false;
            }
        }
    }
}

// Função para desenhar os monstros em movimento na tela
void desenharMonstrosMovendo(MonstroMovendo monstros[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (monstros[i].ativo && monstros[i].node != NULL) {
            DrawRectangleV(monstros[i].posicao, (Vector2){50, 50}, monstros[i].node->monstro.cor);
            DrawText(monstros[i].node->monstro.nome, monstros[i].posicao.x, monstros[i].posicao.y - 20, 10, RAYWHITE);
        }
    }
}

int main() {
    Node* listaMonstros = NULL;
    carregarMonstros("monstros.txt", &listaMonstros);

    // Inicializa a janela do Raylib
    int larguraTela = 800;
    int alturaTela = 600;
    InitWindow(larguraTela, alturaTela, "Monstros Movendo da Direita para Esquerda com Velocidade Diferente");
    SetTargetFPS(60);

    srand(time(NULL));
    
    Padre padre;
    padre.vida = 100;
    padre.posicao = (Vector2){20, alturaTela - 60};
    padre.cor = BLUE;

    // Inicialize as magias com as cores corretas
    Magia magiaFogo = {"Fogo", 1, 0.4, RED};  
    Magia magiaAgua = {"Água", 2, 0.3, BLUE};      
    Magia magiaVento = {"Vento", 3, 0.6, LIGHTGRAY};  

    MagiaAtiva *listaMagias = NULL; // Lista de magias ativas

    // Array para controlar monstros em movimento
    MonstroMovendo monstrosMovendo[MAX_MONSTROS];
    inicializarMonstrosMovendo(monstrosMovendo, MAX_MONSTROS);

    // Controla o tempo para o surgimento dos monstros
    double tempoAnterior = GetTime();

    // Loop principal de jogo
    while (!WindowShouldClose()) {
        double tempoAtual = GetTime();

        // Checa se passou 1 segundo para criar um novo monstro
        if ((tempoAtual - tempoAnterior) >= 1.0) {
            ativarMonstro(monstrosMovendo, MAX_MONSTROS, listaMonstros, larguraTela, alturaTela);
            tempoAnterior = tempoAtual;
        }

        // Atualiza a posição dos monstros em movimento
        atualizarMonstrosMovendo(monstrosMovendo, MAX_MONSTROS);

        // Desenha o padre
        DrawRectangle(padre.posicao.x, padre.posicao.y, 50, 50, padre.cor);

        // Desenha as magias ativas como círculos
        MagiaAtiva *atual = listaMagias;
        while (atual != NULL) {
             DrawCircle(atual->posicao.x, atual->posicao.y, 10, atual->magia.cor);
             atual = atual->proxima;
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha os monstros em movimento
        desenharMonstrosMovendo(monstrosMovendo, MAX_MONSTROS);

        // Checa se o jogador pressiona as teclas para criar magias
        if (IsKeyPressed(KEY_Q)) {
            MagiaAtiva *novaMagia = (MagiaAtiva *)malloc(sizeof(MagiaAtiva));
            novaMagia->magia = magiaFogo;
            novaMagia->posicao.x = padre.posicao.x + 50; // Posição inicial ao lado do padre
            novaMagia->posicao.y = padre.posicao.y;
            novaMagia->proxima = listaMagias;
            listaMagias = novaMagia;
        }
        if (IsKeyPressed(KEY_W)) {
            MagiaAtiva *novaMagia = (MagiaAtiva *)malloc(sizeof(MagiaAtiva));
            novaMagia->magia = magiaAgua;
            novaMagia->posicao.x = padre.posicao.x + 50;
            novaMagia->posicao.y = padre.posicao.y;
            novaMagia->proxima = listaMagias;
            listaMagias = novaMagia;
        }
        if (IsKeyPressed(KEY_E)) {
            MagiaAtiva *novaMagia = (MagiaAtiva *)malloc(sizeof(MagiaAtiva));
            novaMagia->magia = magiaVento;
            novaMagia->posicao.x = padre.posicao.x + 50;
            novaMagia->posicao.y = padre.posicao.y;
            novaMagia->proxima = listaMagias;
            listaMagias = novaMagia;
        }

        // Atualiza as posições das magias ativas e remove as que saem da tela
        MagiaAtiva *anterior = NULL;
        MagiaAtiva *atualMagia = listaMagias;
        while (atualMagia != NULL) {
            atualMagia->posicao.x += atualMagia->magia.velocidade;

            // Verifica se a magia saiu da tela
            if (atualMagia->posicao.x > larguraTela) {
                if (anterior == NULL) {
                    listaMagias = atualMagia->proxima;
                } else {
                    anterior->proxima = atualMagia->proxima;
                }

                MagiaAtiva *magiaParaRemover = atualMagia;
                atualMagia = atualMagia->proxima;
                free(magiaParaRemover);
            } else {
                anterior = atualMagia;
                atualMagia = atualMagia->proxima;
            }
        }

        EndDrawing();
    }

    // Libera memória da lista de monstros e encerra a janela
    liberarLista(listaMonstros);
    CloseWindow();

    return 0;
}
