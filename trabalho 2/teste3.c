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
    int cont = 1;
    int tamanho_pacote;
    bool run = true;
    if(*fila > 0){
        printf("0 - Primeiro da fila: %d\n", *primeiro_da_fila);
        tamanho_link = tamanho_link - *primeiro_da_fila;
        (*fila)--;
    }
    *fila = *fila + quant_pacotes;
    while(*fila > 0 && run){
        tamanho_pacote = gera_pacote();
        printf("%d - tamanho do pacote: %d\n", cont, tamanho_pacote);
        cont++;
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

    printf("\n");

    int x = 5;
    int y = 3;

    double r1 = (double)x / y;
    double r2 = (double)x / (double)y;

    printf("resulta 1: %f\n", r1);
    printf("resulta 2: %f\n", r2);

    
    return 0;
}