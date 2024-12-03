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

    double link = 2 * 64000.0 + 44100.0;
    printf("tamanho do link : %f\n", link);
    double tempo = 1280.0 / link;
    printf("tempo da mensagens passa pelo link: %f\n", tempo);
    return 0;
}