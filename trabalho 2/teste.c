#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

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

int main(){

    srand(2);

    double vetor[100];

    double soma = 0;

    printf("\n");
    for(int i = 0; i < 100; i++){
        vetor[i] = gera_tempo(1.0/100);
        printf("%f ", vetor[i]);
        soma = soma + vetor[i];
    }
    printf("\n");
    printf("Soma total: %f\n", soma);
    printf("Media: %f\n", soma/100);
    return 0;
}