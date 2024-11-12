#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <stdbool.h>

double uniforme()
{
    double u = rand() / ((double)RAND_MAX + 1);
    // u == 0 --> ln(u) <-- problema
    // limitando u entre (0,1]
    u = 1.0 - u;
    return u;
}

double gera_tempo(double l)
{
    return (-1.0 / l) * log(uniforme());
}

int gera_pacote(){
    int var = rand() % 10;
    if(var < 5){
        return 550;
    }else{
        if(var == 9){
            return 1500;
        }else{
            return 40;
        }
    }
}

void preenche_link(int tamanho_link, int quant_pacotes, unsigned long int *fila, int *primeiro_da_fila){
    int tamanho_pacote;
    bool run = true;
    if(*fila > 0){
        printf("Primeiro da fila: %d\n", *primeiro_da_fila);
        tamanho_link = tamanho_link - *primeiro_da_fila;
        (*fila)--;
    }
    *fila = *fila + quant_pacotes;
    while(*fila > 0 && run){
        tamanho_pacote = gera_pacote();
        printf("tamanho do pacote: %d\n", tamanho_pacote);
        if((tamanho_link - tamanho_pacote) >= 0){
            tamanho_link = tamanho_link - tamanho_pacote;
            (*fila)--;
        }else{
            *primeiro_da_fila = tamanho_pacote;
            run = false;
        }
    }
}

int main(){

    srand(1);

    // int vetor[1000];

    // int soma = 0;

    // printf("\n");
    // for(int i = 0; i < 1000; i++){
    //     vetor[i] = (int) gera_tempo(1.0/100);
    //     printf("%d ", vetor[i]);
    //     soma = soma + vetor[i];
    // }
    // printf("\n");
    // printf("Soma total: %d\n", soma);
    // printf("Media: %d\n", soma/1000);

    // for(int i = 0; i < 20; i++){
    //     printf("%d ", tamanho_pacote());
    // }
    // printf("\n");
    // printf("%d", 44100/60);
    // printf("\n");
    // printf("%f", 44100.0/60);

    printf("\n");

    unsigned long int fila = 100;
    int primeiro = 10;
    int tamanho_do_link = 12000;
    int quat = 50;

    preenche_link(tamanho_do_link, quat, &fila, &primeiro);

    printf("tamanho da fila atua: %ld\n", fila);
    printf("novo primeiro da fila: %d\n", primeiro);

    
    printf("\n");
    return 0;
}