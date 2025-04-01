#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> // Para gettimeofday

#define TAM 10000

void preencher_matriz(int **matriz, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = rand() % 10; 
        }
    }
}

void preencher_vetor(int *vetor, int colunas) {
    for (int i = 0; i < colunas; i++) {
        vetor[i] = rand() % 10; 
    }
}

void multiplicar_matriz_vetor_1(int **matriz, int *vetor, int *resultado, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        resultado[i] = 0;
        for (int j = 0; j < colunas; j++) {
            resultado[i] += matriz[i][j] * vetor[j]; // Acesso por linhas
        }
    }
}

void multiplicar_matriz_vetor_2(int **matriz, int *vetor, int *resultado, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        resultado[i] = 0;
    }
    for (int i = 0; i < colunas; i++) {
        for (int j = 0; j < linhas; j++) {
            resultado[j] += matriz[j][i] * vetor[i]; // Acesso por colunas
        }
    }
}

int main() {
    struct timeval start_time, end_time;
    double elapsed_time_1, elapsed_time_2;

    srand(time(0));

    // Alocação dinâmica de memória
    int **matriz = (int **)malloc(TAM * sizeof(int *)); 
    for (int i = 0; i < TAM; i++) {
        matriz[i] = (int *)malloc(TAM * sizeof(int)); 
    }

    int *vetor = (int *)malloc(TAM * sizeof(int));
    int *resultado_1 = (int *)malloc(TAM * sizeof(int));
    int *resultado_2 = (int *)malloc(TAM * sizeof(int));

    // Preencher a matriz e o vetor
    preencher_matriz(matriz, TAM, TAM);
    preencher_vetor(vetor, TAM);

    // Abrir arquivo para salvar os tempos
    FILE *fp = fopen("tempos_execucao.csv", "w");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    fprintf(fp, "Tamanho,Multiplicacao_1,Multiplicacao_2\n");

    for (int i = 1; i <= TAM; i *= 10) {
        // Medir tempo da multiplicação 1
        gettimeofday(&start_time, NULL);
        multiplicar_matriz_vetor_1(matriz, vetor, resultado_1, i, i);
        gettimeofday(&end_time, NULL);
        elapsed_time_1 = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

        // Medir tempo da multiplicação 2
        gettimeofday(&start_time, NULL);
        multiplicar_matriz_vetor_2(matriz, vetor, resultado_2, i, i);
        gettimeofday(&end_time, NULL);
        elapsed_time_2 = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

        // Salvar no arquivo CSV
        fprintf(fp, "%d,%f,%f\n", i, elapsed_time_1, elapsed_time_2);

        printf("Tamanho %d: Multiplicacao 1 = %f s | Multiplicacao 2 = %f s\n", i, elapsed_time_1, elapsed_time_2);
    }

    fclose(fp);

    // Liberação da memória alocada dinamicamente
    for (int i = 0; i < TAM; i++) {
        free(matriz[i]);
    }
    free(matriz);
    free(vetor);
    free(resultado_1);
    free(resultado_2);

    printf("Dados salvos em 'tempos_execucao.csv'. Use Python ou GNUplot para gerar o gráfico.\n");

    return 0;
}