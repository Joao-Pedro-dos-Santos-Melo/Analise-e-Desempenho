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

typedef struct
{
    double tempo_entrada;
    double tempo_chegada_pacote;
    double tempo_saida;
} chamada;

typedef struct
{
    double tempo_saida;
} pacote;


// Nó da lista encadeada
typedef struct Node {
    chamada data;
    struct Node* next;
} Node;

typedef struct NodePacote {
    pacote data;
    struct NodePacote* next;
} NodePacote;

// Função para criar um novo nó
Node* criarNo(chamada c) {
    Node* novoNo = (Node*)malloc(sizeof(Node));
    novoNo->data = c;
    novoNo->next = NULL;
    return novoNo;
}

// Função para adicionar um nó no final da lista
void adicionarNoFinal(Node** head, chamada c) {
    Node* novoNo = criarNo(c);
    if (*head == NULL) {
        *head = novoNo;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novoNo;
    }
}

// Funções para manipular a lista de pacotes
NodePacote* criarNoPacote(pacote p) {
    NodePacote* novoNo = (NodePacote*)malloc(sizeof(NodePacote));
    novoNo->data = p;
    novoNo->next = NULL;
    return novoNo;
}

void adicionarNoFinalPacote(NodePacote** head, pacote p) {
    NodePacote* novoNo = criarNoPacote(p);
    if (*head == NULL) {
        *head = novoNo;
    } else {
        NodePacote* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novoNo;
    }
}

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

double gera_pacote(){
    double var = rand() % 10;
    if(var < 5.0){
        return 550.0;
    }else{
        if(var == 9.0){
            return 1500.0;
        }else{
            return 40.0;
        }
    }
}

double tempo_do_pacote (double pacote, double tamanho_link){
    double tempo = pacote / tamanho_link;
    return tempo;
}

int main()
{
    srand(1);
    double ocupacaoD;
    printf("Informe a Ocupação desejada: ");
    scanf("%lf", &ocupacaoD);


    double tempo_simulacao;
    printf("Informe o tempo de simulacao (s): ");
    scanf("%lF", &tempo_simulacao);

    // 50 * 550 + 40 * 40 + 10 * 1500 = 44100 -> pacotes normais
    // 64000 -> pacotes da video chamada
    // Como vai ter em media duas chamadas ao mesmo tempo, a quantidade de Bytes por segundo vai ser:
    // 64000 + 64000 + 44100 = 172100
    double tamanho_do_link = 172100.0 / (ocupacaoD * 0.01);
    printf("tamanho do link : %lF\n", tamanho_do_link);
    printf("-----------------------------------------\n");

    double parametro_gera_pacote = 100.0;

    // parametros das chamadas
    double parametro_chegada_chamada = 1.0/30.0;
    double parametro_duracao_chamada = 1.0/60.0;

    // 1 seg passa 50 pacotes, se em 1 seg passa 64000 Bytes da chamada, entao 1 pacote da chamada tem 64000/50 = 1280 Bytes
    double tamanho_pacote_chamada = 1280;
    double tempo_pacote_chamada = 0.02;

    // lista de chamadas
    Node* chamadas = NULL;

    // lista de pacotes
    NodePacote* pacotes = NULL;

    //tempo que o primeiro pacote chegara no sistema
    double tempo_chegada = gera_tempo(parametro_gera_pacote);
    double tempo_chegada_chamada = gera_tempo(parametro_chegada_chamada);

    double tempo_decorrido = 0.0;

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
    little en_chamada;
    little ew_chamada_chegadas;
    little ew_chamada_saidas;
    inicia_little(&en);
    inicia_little(&ew_chegadas);
    inicia_little(&ew_saidas);
    inicia_little(&en_chamada);
    inicia_little(&ew_chamada_chegadas);
    inicia_little(&ew_chamada_saidas);

    // variavel da partição
    double tempo_particao = 100.0;

    // variaveis dos parametros
    double en_final;
    double ew_final;
    double lambda;
    double erroLittle;
    double ocupacao;

    // Abrir um arquivo para escrita
    FILE *file = fopen("simulacao99.csv", "w");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Cabeçalho do arquivo
    fprintf(file, "Tempo;Ocupação;E[N];E[W];Erro Little;Lambda\n");


    double p;
    double tempo_saida_chamda;
    double tempo_chegada_pacote_chamada;
    double tempo_saida_pacote_chamada;

    // Simulação
    while (tempo_decorrido <= tempo_simulacao)
    {
        
        if(chamadas == NULL){ //setando os dados da chamada
            tempo_saida_chamda = DBL_MAX;
            tempo_chegada_pacote_chamada = DBL_MAX;
        }else{
            tempo_saida_chamda = chamadas->data.tempo_saida;
            tempo_chegada_pacote_chamada = chamadas->data.tempo_chegada_pacote; 
        }
        if(pacotes == NULL){ // setando os dados do pacote
            tempo_saida_pacote_chamada = DBL_MAX;
        }else{
            tempo_saida_pacote_chamada = pacotes->data.tempo_saida;
        }

        
        // Pega o tempo do proximo evento
        tempo_decorrido = min(min(min(min(min(min(tempo_chegada, tempo_saida), tempo_particao), 
        tempo_chegada_chamada), tempo_saida_chamda), tempo_chegada_pacote_chamada), tempo_saida_pacote_chamada);


        if (tempo_decorrido == tempo_particao) // Evento captura de dados
        {
            // printf("tempo particao: %lF\n", tempo_particao);
            // printf("=====================\n");
            // getchar();

            // Atualiza area no momento da partição
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
            fprintf(file, "%.f;%.6f;%.6f;%.6f;%.20f;%.6f\n", tempo_decorrido, ocupacao, en_final, ew_final, erroLittle, lambda);

            // Define o tempo da nova partição
            tempo_particao += 100.0;
        } else if(tempo_decorrido == tempo_chegada_chamada){ //Evento Chegada Chamada

            chamada nova;
            nova.tempo_entrada = tempo_decorrido;//salva o tempo de chegada da chamada
            nova.tempo_saida = tempo_decorrido + gera_tempo(parametro_duracao_chamada);// define o tempo de saida da chamada
            nova.tempo_chegada_pacote = tempo_decorrido + tempo_pacote_chamada; //tempo de chegada do pacote

            adicionarNoFinal(&chamadas, nova);

            

            tempo_chegada_chamada = tempo_decorrido + gera_tempo(parametro_chegada_chamada);


        } else if (tempo_decorrido == tempo_chegada) //Evento de chegada de pacote web
        {

            tempo_chegada = tempo_decorrido + gera_tempo(parametro_gera_pacote);
            // sistema esta ocioso?
            if (!fila)
            {
                p = gera_pacote();
                tempo_saida = tempo_decorrido + tempo_do_pacote(p, tamanho_do_link);

                soma_ocupacao += tempo_saida - tempo_decorrido;
            }
            fila++;
            fila_max = fila > fila_max ? fila : fila_max;

            /**
             * little
             */
            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
            en.num_eventos++;
            en.tempo_anterior = tempo_decorrido;

            ew_chegadas.soma_areas += (tempo_decorrido - ew_chegadas.tempo_anterior) * ew_chegadas.num_eventos;
            ew_chegadas.num_eventos++;
            ew_chegadas.tempo_anterior = tempo_decorrido;

        }else if(tempo_decorrido == tempo_chegada_pacote_chamada){ // Evento de chagada de pacote da chamada

           

            chamadas->data.tempo_chegada_pacote = tempo_decorrido + tempo_pacote_chamada;
            // sistema esta ocioso?
            if (!fila)
            {
                pacote n;
                n.tempo_saida = tempo_decorrido + tempo_do_pacote(tamanho_pacote_chamada, tamanho_do_link);
                adicionarNoFinalPacote(&pacotes, n);

                soma_ocupacao += n.tempo_saida - tempo_decorrido;
            }
            fila++;
            fila_max = fila > fila_max ? fila : fila_max;

            /**
             * little
             */
            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
            en.num_eventos++;
            en.tempo_anterior = tempo_decorrido;

            ew_chegadas.soma_areas += (tempo_decorrido - ew_chegadas.tempo_anterior) * ew_chegadas.num_eventos;
            ew_chegadas.num_eventos++;
            ew_chegadas.tempo_anterior = tempo_decorrido;

            //little so da chamada
            en_chamada.soma_areas += (tempo_decorrido - en_chamada.tempo_anterior) * en_chamada.num_eventos;
            en_chamada.num_eventos++;
            en_chamada.tempo_anterior = tempo_decorrido;

            ew_chamada_chegadas.soma_areas += (tempo_decorrido - ew_chamada_chegadas.tempo_anterior) * ew_chamada_chegadas.num_eventos;
            ew_chamada_chegadas.num_eventos++;
            ew_chamada_chegadas.tempo_anterior = tempo_decorrido;
        }
        else if(tempo_decorrido == tempo_saida) // Evento de Saida pacote web
        {

            fila--;
            tempo_saida = DBL_MAX;
            // tem mais requisicoes na fila?
            if (fila)
            {
                p = gera_pacote();
                tempo_saida = tempo_decorrido + tempo_do_pacote(p, tamanho_do_link);

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

        }else if(tempo_decorrido == tempo_saida_pacote_chamada){ // Evento de saida de pacote chamada
            fila--;

            

            /**
             * little
             */
            en.soma_areas += (tempo_decorrido - en.tempo_anterior) * en.num_eventos;
            en.num_eventos--;
            en.tempo_anterior = tempo_decorrido;

            ew_saidas.soma_areas += (tempo_decorrido - ew_saidas.tempo_anterior) * ew_saidas.num_eventos;
            ew_saidas.num_eventos++;
            ew_saidas.tempo_anterior = tempo_decorrido;

            //little chamada
            en_chamada.soma_areas += (tempo_decorrido - en_chamada.tempo_anterior) * en_chamada.num_eventos;
            en_chamada.num_eventos--;
            en_chamada.tempo_anterior = tempo_decorrido;

            ew_chamada_saidas.soma_areas += (tempo_decorrido - ew_chamada_saidas.tempo_anterior) * ew_chamada_saidas.num_eventos;
            ew_chamada_saidas.num_eventos++;
            ew_chamada_saidas.tempo_anterior = tempo_decorrido;

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
    printf("EW chegadas: %ld\n", ew_chegadas.num_eventos);
     printf("EW saidas: %ld\n", ew_saidas.num_eventos);
    printf("Fila final: %ld\n", fila);
    printf("Maior tamanho de fila alcancado: %ld\n", fila_max);
    printf("Ocupacao: %lF\n", ocupacao);
    printf("E[N]: %lF\n", en_final);
    printf("E[W]: %lF\n", ew_final);
    printf("lambda: %lF\n", lambda);
    printf("Erro de Little: %.20lF\n", erroLittle);

    // Salva os resultados finais no arquivo
    fprintf(file, "%.f;%.6f;%.6f;%.6f;%.20f;%.6f\n", tempo_decorrido, ocupacao, en_final, ew_final, erroLittle, lambda);
    // Fechar o arquivo
    fclose(file);

    return 0;
}