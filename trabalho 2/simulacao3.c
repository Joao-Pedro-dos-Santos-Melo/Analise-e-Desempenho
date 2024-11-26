#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <stdbool.h>

typedef struct
{
    unsigned long int num_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

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

double min(double n1, double n2)
{
    if (n1 < n2)
        return n1;
    return n2;
}

void inicia_little(little *n)
{
    n->num_eventos = 0;
    n->soma_areas = 0.0;
    n->tempo_anterior = 0.0;
}

void printa_particao(double t_chegada, double ocupacao, double en, double ew, double erroL, double lambda)
{
    printf("Tempo de chegada: %d\n", (int)t_chegada);
    printf("Ocupacao: %f\n", ocupacao);
    printf("E[N]: %f\n", en);
    printf("E[W]: %f\n", ew);
    printf("lambda: %lF\n", lambda);
    printf("Erro de Little: %.20lF\n", erroL);
    printf("-------------------------------------------\n");
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

double tempo_do_pacote (int pacote, int tamanho_link){
    double tempo = (double)pacote / (double)tamanho_link;
    return tempo;
}

int main()
{
    srand(1);
    int ocupacaoD;
    printf("Informe a Ocupação desejada: ");
    scanf("%d", &ocupacaoD);


    double tempo_simulacao;
    printf("Informe o tempo de simulacao (s): ");
    scanf("%lF", &tempo_simulacao);

    // 50 * 550 + 40 * 40 + 10 * 1500 = 44100
    int tamanho_do_link = 44100 / ocupacaoD;

    double parametro_gera_pacote = 1.0/100;
    int quantidade_de_pacotes = (int) gera_tempo(parametro_gera_pacote);

    double tempo_decorrido = 0.0;

    double tempo_chegada = 0.0;
    double tempo_saida = DBL_MAX;

    unsigned long int fila = 0;
    unsigned long int fila_max = 0;

    double soma_ocupacao = 0.0;

    /**
     * variaveis little
     */

    little en;
    little ew_chegadas;
    little ew_saidas;
    inicia_little(&en);
    inicia_little(&ew_chegadas);
    inicia_little(&ew_saidas);

    // variavel da partição
    double tempo_particao = 100;

    // variaveis dos parametros
    double en_final;
    double ew_final;
    double lambda;
    double erroLittle;
    double ocupacao;

    // Abrir um arquivo para escrita
    //FILE *file = fopen("simulacao99.csv", "w");

    // if (file == NULL) {
    //     perror("Erro ao abrir o arquivo");
    //     return 1;
    // }

    // Cabeçalho do arquivo
    //fprintf(file, "Tempo;Ocupação;E[N];E[W];Erro Little;Lambda\n");


    int primeiro_da_fila = 0;
    int pacote;
    // Simulação
    while (tempo_decorrido <= tempo_simulacao)
    {
        // Pega o tempo do proximo evento
        tempo_decorrido = min(min(tempo_chegada, tempo_saida), tempo_particao);


        if (tempo_decorrido == tempo_particao) // Evento captura de dados
        {
            // Atuaiza area no momento da partição
            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
            en.tempo_anterior = tempo_decorrido;

            ew_chegadas.soma_areas += (tempo_decorrido - ew_chegadas.tempo_anterior) * ew_chegadas.num_eventos;
            ew_chegadas.tempo_anterior = tempo_decorrido;

            ew_saidas.soma_areas += (tempo_decorrido - ew_saidas.tempo_anterior) * ew_saidas.num_eventos;
            ew_saidas.tempo_anterior = tempo_decorrido;

            //calcula os parametros da partiçaõ
            en_final = en.soma_areas / tempo_decorrido;
            ew_final = (ew_chegadas.soma_areas - ew_saidas.soma_areas) / ew_chegadas.num_eventos;
            lambda = ew_chegadas.num_eventos / tempo_decorrido;
            erroLittle = en_final - lambda * ew_final;
            ocupacao = soma_ocupacao / tempo_decorrido;

            // Printa os resultado da atuais da partição
            printa_particao(tempo_decorrido , ocupacao, en_final, ew_final, erroLittle, lambda);

            // Salva no arquivo os resultados da partição
            //fprintf(file, "%.f;%.6f;%.6f;%.6f;%.20f;%.6f\n", tempo_decorrido, ocupacao, en_final, ew_final, erroLittle, lambda);

            // Define o tempo da nova partição
            tempo_particao += 100;
        }
        else if (tempo_decorrido == tempo_chegada) //Evento de chegada
        {
            quantidade_de_pacotes = (int) gera_tempo(parametro_gera_pacote);
            // sistema esta ocioso?
            if (!fila)
            {
                pacote = gera_pacote();
                tempo_saida = tempo_decorrido + tempo_do_pacote(pacote, tamanho_do_link);

                soma_ocupacao += tempo_saida - tempo_decorrido;
            }
            fila += quantidade_de_pacotes;
            fila_max = fila > fila_max ? fila : fila_max;

            tempo_chegada = tempo_decorrido + 1.0;

            /**
             * little
             */
            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
            en.num_eventos+= quantidade_de_pacotes;
            en.tempo_anterior = tempo_decorrido;

            ew_chegadas.soma_areas += (tempo_decorrido - ew_chegadas.tempo_anterior) * ew_chegadas.num_eventos;
            ew_chegadas.num_eventos+= quantidade_de_pacotes;
            ew_chegadas.tempo_anterior = tempo_decorrido;
        }
        else // Evento de Saida
        {
            fila--;
            tempo_saida = DBL_MAX;
            // tem mais requisicoes na fila?
            if (fila)
            {
                pacote = gera_pacote();
                tempo_saida = tempo_decorrido + tempo_do_pacote(pacote, tamanho_do_link);

                soma_ocupacao += tempo_saida - tempo_decorrido;
            }

            /**
             * little
             */
            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
            en.num_eventos--;
            en.tempo_anterior = tempo_decorrido;

            ew_saidas.soma_areas += (tempo_decorrido - ew_saidas.tempo_anterior) * ew_saidas.num_eventos;
            ew_saidas.num_eventos++;
            ew_saidas.tempo_anterior = tempo_decorrido;
        }
    }

    //Calculos das areas que pode ter faltado
    ew_chegadas.soma_areas += (tempo_decorrido - ew_chegadas.tempo_anterior) * ew_chegadas.num_eventos;
    ew_saidas.soma_areas += (tempo_decorrido - ew_saidas.tempo_anterior) * ew_saidas.num_eventos;

    //Calculos das parametros finais
    ocupacao = soma_ocupacao / tempo_decorrido;
    en_final = en.soma_areas / tempo_decorrido;
    ew_final = (ew_chegadas.soma_areas - ew_saidas.soma_areas) / ew_chegadas.num_eventos;
    lambda = ew_chegadas.num_eventos / tempo_decorrido;
    erroLittle = en_final - lambda * ew_final;
    
    // Printa os resultado finais
    printf("Fila final: %ld\n", fila);
    printf("Maior tamanho de fila alcancado: %ld\n", fila_max);
    printf("Ocupacao: %lF\n", ocupacao);
    printf("E[N]: %lF\n", en_final);
    printf("E[W]: %lF\n", ew_final);
    printf("lambda: %lF\n", lambda);
    printf("Erro de Little: %.20lF\n", erroLittle);

    // Salva os resultados finais no arquivo
    //fprintf(file, "%.2f;%.6f;%.6f;%.6f;%.20f;%.6f\n", tempo_decorrido, ocupacao, en_final, ew_final, erroLittle, lambda);
    // Fechar o arquivo
    //fclose(file);

    return 0;
}