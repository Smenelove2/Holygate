// holygate.c
#include "holygate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *CriarMonstro(char *nome, float velocidade, Color cor) {
    Node *novo = (Node *)malloc(sizeof(Node));
    strcpy(novo->monstro.nome, nome);
    novo->monstro.vida = 100;
    novo->monstro.velocidade = velocidade;
    novo->monstro.posicao = (Vector2){1366, 710 - 50}; // Canto inferior direito, ajustado para não sair da tela
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