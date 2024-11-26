#include <stdio.h>
#include <stdlib.h>

int main (){
    float saldo = 40000;
    float saldo1 = 40000;
    float saldo05 = 40000;
    int i = 10 * 12;
    int troca = 1;
    for(int j = 1; j < i; j++){
        if(troca == 1){
            saldo = saldo + saldo * 0.005;
            troca = 0;
        }else{
            saldo = saldo + saldo * 0.01;
            troca = 1;
        }
        saldo1 = saldo1 + saldo1 * 0.01;
        saldo05 = saldo05 + saldo05 * 0.005;
        
    }

    printf("Saldo final alternado: %.2f\n", saldo);
    printf("Saldo final 1%: %.2f\n", saldo1);
    printf("Saldo final 0.5%: %.2f\n", saldo05);
}