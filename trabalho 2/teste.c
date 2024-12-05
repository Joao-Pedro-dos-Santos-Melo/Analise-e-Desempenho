#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

#include <stdio.h>
#include <stdlib.h>

// Definições de estruturas
typedef struct {
    double tempo_entrada;
    double tempo_chegada_pacote;
    double tempo_saida;
} chamada;

typedef struct Node {
    chamada data;
    struct Node* next;
} Node;

// Funções de lista encadeada
Node* criarNo(chamada c) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
    novoNo->data = c;
    novoNo->next = NULL;
    return novoNo;
}

void adicionarNoFinal(Node** head, chamada c) {
    Node* novoNo = criarNo(c);
    if (*head == NULL) {
        *head = novoNo;
    } else {
        Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = novoNo;
    }
}

void imprimirLista(Node* head) {
    Node* temp = head;
    while (temp != NULL) {
        printf("Chamada: entrada=%.2f, chegada=%.2f, saida=%.2f\n", 
               temp->data.tempo_entrada, 
               temp->data.tempo_chegada_pacote, 
               temp->data.tempo_saida);
        temp = temp->next;
    }
}

// Funções pedidas
void moverPrimeiraParaUltima(Node** head) {
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }

    Node* temp = *head;
    *head = (*head)->next;
    temp->next = NULL;

    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = temp;
}

void removerPrimeiraChamada(Node** head) {
    if (*head == NULL) {
        return;
    }

    Node* temp = *head;
    *head = (*head)->next;
    free(temp);
}

// Programa principal
int main() {
    Node* lista_chamadas = NULL;

    chamada c1 = {.tempo_entrada = 1.0, .tempo_chegada_pacote = 2.0, .tempo_saida = 3.0};
    chamada c2 = {.tempo_entrada = 4.0, .tempo_chegada_pacote = 5.0, .tempo_saida = 6.0};
    chamada c3 = {.tempo_entrada = 7.0, .tempo_chegada_pacote = 8.0, .tempo_saida = 9.0};

    adicionarNoFinal(&lista_chamadas, c1);
    adicionarNoFinal(&lista_chamadas, c2);
    adicionarNoFinal(&lista_chamadas, c3);

    printf("Lista original:\n");
    imprimirLista(lista_chamadas);

    moverPrimeiraParaUltima(&lista_chamadas);
    printf("\nDepois de mover a primeira para o final:\n");
    imprimirLista(lista_chamadas);

    removerPrimeiraChamada(&lista_chamadas);
    printf("\nDepois de remover a primeira chamada:\n");
    imprimirLista(lista_chamadas);

    return 0;
}
