#include "raylib.h"
#include "holygate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Criar novo mosntro
Node *CriarMonstro(char *nome, float velocidade, Color cor);
// Adicionar monstro à lista
void AdicionarMonstro(Node **head, Node *novo);
// Remover monstro da lista
void RemoverMonstro(Node **head, Node *monstro);
// Atualizar a posição dos monstros
void AtualizarMonstros(Node **head);
// Desenhar os monstros na tela
void DesenharMonstros(Node *head);

const int larguraTela = 1366;
const int alturaTela = 690;

int main(void) {

    InitWindow(larguraTela, alturaTela, "HolyGate Menu");
    
    Node *monstros = NULL;
    float tempoDecorrido = 0.0f;

    srand(time(NULL)); // Inicializar a semente aleatória

    Padre padre;
    padre.vida = 100;
    padre.posicao = (Vector2){20, alturaTela - 60};
    padre.cor = BLUE;

    // Inicialize as magias com as cores corretas
    Magia magiaFogo = {"Fogo", 1, 0.4, RED};  
    Magia magiaAgua = {"Água", 2, 0.3, BLUE};      
    Magia magiaVento = {"Vento", 3, 0.6, LIGHTGRAY};  

    MagiaAtiva *listaMagias = NULL; // Lista de magias ativas

    // Definindo as cores e variáveis de interface
    Color corBotaoNormal = DARKGRAY;
    Color corBotaoHover = (Color){200, 200, 200, 255};

    int larguraBotao = 200;
    int alturaBotao = 50;
    Vector2 posicaoBotao[4];
    int larguraBotaoVoltar = 100;
    int alturaBotaoVoltar = 40;
    Vector2 posicaoBotaoVoltar = {20, 20};

    // Posicionando os botões do menu
    for (int i = 0; i < 4; i++) {
        posicaoBotao[i].x = (larguraTela - larguraBotao) / 2;
        posicaoBotao[i].y = (alturaTela / 2) + (i - 1.5) * (alturaBotao + 10);
    }

    int estadoTela = 0;
    bool fecharPrograma = false;
    bool jogoAtivo = false;

    while (!WindowShouldClose() && !fecharPrograma) {
        Vector2 posicaoMouse = GetMousePosition();

        // Lógica para o estado de tela "Jogar"
        if (estadoTela == 1) {
            if (!jogoAtivo) {
                jogoAtivo = true;
            }
            
            float deltaTime = GetFrameTime();
            tempoDecorrido += deltaTime;

            // Gerar um novo monstro a cada 2 segundos
            if (tempoDecorrido >= 2.0f) {
                tempoDecorrido = 0.0f;

                // Criar um novo monstro com atributos aleatórios
                char nome[50] = "Monstro";
                float velocidade = (float)(rand() % 3 + 2); // Velocidade aleatória entre 2 e 4
                Color cor = (Color){rand() % 256, rand() % 256, rand() % 256, 255};

                Node *novoMonstro = CriarMonstro(nome, velocidade, cor);
                AdicionarMonstro(&monstros, novoMonstro);
            }
    
            // Atualizar os monstros
            AtualizarMonstros(&monstros);

            // Detecta se as teclas Q, W, ou E são pressionadas para lançar magias
            if (IsKeyPressed(KEY_Q)) {
                // Cria uma nova magia de fogo
                MagiaAtiva *novaMagia = (MagiaAtiva *)malloc(sizeof(MagiaAtiva));
                novaMagia->magia = magiaFogo;
                novaMagia->posicao.x = padre.posicao.x + 50;// A posição inicial é a posição do padre
                novaMagia->posicao.y = padre.posicao.y;
                novaMagia->proxima = listaMagias;
                listaMagias = novaMagia;
            }
            if (IsKeyPressed(KEY_W)) {
                // Cria uma nova magia de água
                MagiaAtiva *novaMagia = (MagiaAtiva *)malloc(sizeof(MagiaAtiva));
                novaMagia->magia = magiaAgua;
                novaMagia->posicao.x = padre.posicao.x + 50;// A posição inicial é a posição do padre
                novaMagia->posicao.y = padre.posicao.y;
                novaMagia->proxima = listaMagias;
                listaMagias = novaMagia;
            }
            if (IsKeyPressed(KEY_E)) {
                // Cria uma nova magia de vento
                MagiaAtiva *novaMagia = (MagiaAtiva *)malloc(sizeof(MagiaAtiva));
                novaMagia->magia = magiaVento;
                novaMagia->posicao.x = padre.posicao.x + 50;// A posição inicial é a posição do padre
                novaMagia->posicao.y = padre.posicao.y;
                novaMagia->proxima = listaMagias;
                listaMagias = novaMagia;
            }

            // Atualiza as posições das magias ativas e remove as que saem da tela
            MagiaAtiva *anterior = NULL;
            MagiaAtiva *atual = listaMagias;
            while (atual != NULL) {
                // Movimenta a magia para a direita
                atual->posicao.x += atual->magia.velocidade;

                // Verifica se a magia saiu da tela
                if (atual->posicao.x > larguraTela) {
                    // Remove a magia da lista
                    if (anterior == NULL) {
                        // A magia a ser removida é a primeira da lista
                        listaMagias = atual->proxima;
                    } else {
                        // A magia a ser removida está no meio ou no fim da lista
                        anterior->proxima = atual->proxima;
                    }

                    // Libera a memória da magia removida
                    MagiaAtiva *magiaParaRemover = atual;
                    atual = atual->proxima;
                    free(magiaParaRemover);
                } else {
                    // Continua para a próxima magia
                    anterior = atual;
                    atual = atual->proxima;
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (estadoTela == 0) {
            // Tela do Menu Principal
            DrawText("HolyGate", larguraTela / 2 - MeasureText("HolyGate", 40) / 2, 50, 40, YELLOW);

            const char *botoes[4] = {"JOGAR", "MONSTROS", "LEADERBOARD", "SAIR"};
            for (int i = 0; i < 4; i++) {
                bool hover = (posicaoMouse.x >= posicaoBotao[i].x && posicaoMouse.x <= posicaoBotao[i].x + larguraBotao &&
                              posicaoMouse.y >= posicaoBotao[i].y && posicaoMouse.y <= posicaoBotao[i].y + alturaBotao);

                Color corBotao = hover ? corBotaoHover : corBotaoNormal;
                DrawRectangle(posicaoBotao[i].x, posicaoBotao[i].y, larguraBotao, alturaBotao, corBotao);
                DrawText(botoes[i], posicaoBotao[i].x + larguraBotao / 2 - MeasureText(botoes[i], 20) / 2, posicaoBotao[i].y + 10, 20, RAYWHITE);

                if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (i == 0) {
                        estadoTela = 1; // Vai para a tela "Jogar"
                    } else if (i == 1) {
                        estadoTela = 2; // Vai para a tela "Monstros"
                    } else if (i == 2) {
                        estadoTela = 3; // Vai para a tela "Leaderboard"
                    } else if (i == 3) {
                        fecharPrograma = true;
                    }
                }
            }
        } else if (estadoTela == 1) {
            // Tela "Jogar"
            DrawText("Você está em JOGAR", larguraTela / 2 - MeasureText("Você está em JOGAR", 40) / 2, 50, 40, YELLOW);

            // Desenha o padre
            DrawRectangle(padre.posicao.x, padre.posicao.y, 50, 50, padre.cor);

            // Desenha as magias ativas como círculos
            MagiaAtiva *atual = listaMagias;
            while (atual != NULL) {
                DrawCircle(atual->posicao.x, atual->posicao.y, 10, atual->magia.cor);
                atual = atual->proxima;
            }

            // Botão "Voltar"
            bool hoverVoltar = (posicaoMouse.x >= posicaoBotaoVoltar.x && posicaoMouse.x <= posicaoBotaoVoltar.x + larguraBotaoVoltar &&
                                posicaoMouse.y >= posicaoBotaoVoltar.y && posicaoMouse.y <= posicaoBotaoVoltar.y + alturaBotaoVoltar);

            Color corBotaoVoltar = hoverVoltar ? corBotaoHover : corBotaoNormal;
            DrawRectangle(posicaoBotaoVoltar.x, posicaoBotaoVoltar.y, larguraBotaoVoltar, alturaBotaoVoltar, corBotaoVoltar);
            DrawText("Voltar", posicaoBotaoVoltar.x + larguraBotaoVoltar / 2 - MeasureText("Voltar", 20) / 2, posicaoBotaoVoltar.y + 10, 20, RAYWHITE);
            
            DesenharMonstros(monstros);

            if (hoverVoltar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                estadoTela = 0; // Volta para o menu principal
                jogoAtivo = false;
            }
        } else if (estadoTela == 2) {
            // Tela "Monstros"
            DrawText("Você está em MONSTROS", larguraTela / 2 - MeasureText("Você está em MONSTROS", 40) / 2, 50, 40, YELLOW);

            // Botão "Voltar"
            bool hoverVoltar = (posicaoMouse.x >= posicaoBotaoVoltar.x && posicaoMouse.x <= posicaoBotaoVoltar.x + larguraBotaoVoltar &&
                                posicaoMouse.y >= posicaoBotaoVoltar.y && posicaoMouse.y <= posicaoBotaoVoltar.y + alturaBotaoVoltar);

            Color corBotaoVoltar = hoverVoltar ? corBotaoHover : corBotaoNormal;
            DrawRectangle(posicaoBotaoVoltar.x, posicaoBotaoVoltar.y, larguraBotaoVoltar, alturaBotaoVoltar, corBotaoVoltar);
            DrawText("Voltar", posicaoBotaoVoltar.x + larguraBotaoVoltar / 2 - MeasureText("Voltar", 20) / 2, posicaoBotaoVoltar.y + 10, 20, RAYWHITE);

            if (hoverVoltar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                estadoTela = 0; // Volta para o menu principal
            }
        } else if (estadoTela == 3) {
            // Tela "Leaderboard"
            DrawText("Você está em LEADERBOARD", larguraTela / 2 - MeasureText("Você está em LEADERBOARD", 40) / 2, 50, 40, YELLOW);

            // Botão "Voltar"
            bool hoverVoltar = (posicaoMouse.x >= posicaoBotaoVoltar.x && posicaoMouse.x <= posicaoBotaoVoltar.x + larguraBotaoVoltar &&
                                posicaoMouse.y >= posicaoBotaoVoltar.y && posicaoMouse.y <= posicaoBotaoVoltar.y + alturaBotaoVoltar);

            Color corBotaoVoltar = hoverVoltar ? corBotaoHover : corBotaoNormal;
            DrawRectangle(posicaoBotaoVoltar.x, posicaoBotaoVoltar.y, larguraBotaoVoltar, alturaBotaoVoltar, corBotaoVoltar);
            DrawText("Voltar", posicaoBotaoVoltar.x + larguraBotaoVoltar / 2 - MeasureText("Voltar", 20) / 2, posicaoBotaoVoltar.y + 10, 20, RAYWHITE);

            if (hoverVoltar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                estadoTela = 0; // Volta para o menu principal
            }
        }

        EndDrawing();
    }

    // Limpa a memória alocada para as magias
    MagiaAtiva *atual = listaMagias;
    while (atual != NULL) {
        MagiaAtiva *proxima = atual->proxima;
        free(atual);
        atual = proxima;
    }
    
    while (monstros != NULL) {
        Node *temp = monstros;
        monstros = monstros->proximo;
        free(temp);
    }

    CloseWindow();
    return 0;
}

// Função para criar um novo monstro
Node *CriarMonstro(char *nome, float velocidade, Color cor) {
    Node *novo = (Node *)malloc(sizeof(Node));
    strcpy(novo->monstro.nome, nome);
    novo->monstro.vida = 100;
    novo->monstro.velocidade = velocidade;
    novo->monstro.posicao = (Vector2){larguraTela, alturaTela - 60}; // Canto inferior direito, ajustado para não sair da tela
    novo->monstro.cor = cor;
    novo->proximo = NULL;
    novo->anterior = NULL;
    return novo;
}

// Adicionar monstro à lista
void AdicionarMonstro(Node **head, Node *novo) {
    if (*head == NULL) {
        *head = novo;
    } else {
        Node *temp = *head;
        while (temp->proximo != NULL) {
            temp = temp->proximo;
        }
        temp->proximo = novo;
        novo->anterior = temp;
    }
}

// Remover monstro da lista
void RemoverMonstro(Node **head, Node *monstro) {
    if (*head == NULL || monstro == NULL) return;

    if (*head == monstro) *head = monstro->proximo;

    if (monstro->proximo != NULL) monstro->proximo->anterior = monstro->anterior;
    if (monstro->anterior != NULL) monstro->anterior->proximo = monstro->proximo;

    free(monstro);
}

// Atualizar a posição dos monstros
void AtualizarMonstros(Node **head) {
    Node *temp = *head;
    while (temp != NULL) {
        temp->monstro.posicao.x -= temp->monstro.velocidade;

        // Se o monstro sair da tela, removê-lo
        if (temp->monstro.posicao.x < -50) { // Ajuste conforme necessário
            Node *aRemover = temp;
            temp = temp->proximo;
            RemoverMonstro(head, aRemover);
        } else {
            temp = temp->proximo;
        }
    }
}

// Desenhar os monstros na tela
void DesenharMonstros(Node *head) {
    Node *temp = head;
    while (temp != NULL) {
        DrawRectangleV(temp->monstro.posicao, (Vector2){50, 50}, temp->monstro.cor); // Desenhar o monstro como um retângulo
        temp = temp->proximo;
    }
}

