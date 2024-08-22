#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//comando para compilar
//gcc exponencial.c -lm -o exponecial
//comando apra executar
//./exponencial

double uniforme(){
    double u = rand() / ((double) RAND_MAX + 1);
    //u == 0 --> ln(u) <-- n pode
    //limitando u entre (0, 1]
    u = 1.0 - u;
    return u;
}

int main (){
    //Na Exponencial , E[X] = 1/l
    //Exemplo: suponha tempo medio = 5 seg
    //E[X] = 1/l = 5 seg
    //Assim, l = 1/5 = 0.2
    double l;
    printf("Infomer o tempo médio: ");
    scanf("%lF", &l);
    l = 1.0/l;

    //iniciando a sequencia pseudo aleatoria
    int semente = time(NULL);
    srand(semente);

    //geracao dos valores
    double qtd_valores_gerados = 10000000;
    double soma = 0.0;

    for(int i =0; i<qtd_valores_gerados; i++){
        double valor = (-1.0/l) * log(uniforme());
        //printf("%lF\n", valor);
        //getchar();
        soma += valor;
    }

    printf("Media dos Valores gerados: %lF\n", (soma/qtd_valores_gerados));

    return 0;
}