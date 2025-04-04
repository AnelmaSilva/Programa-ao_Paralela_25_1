#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <omp.h>

#define N 100000000

double time_diff(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
}

double soma_vetores() {
    double *a = (double*)malloc(N * sizeof(double));
    double *b = (double*)malloc(N * sizeof(double));
    double *c = (double*)malloc(N * sizeof(double));

    // Inicialização paralela sem especificação de threads
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        a[i] = i * 1.0;
        b[i] = i * 2.0;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Soma paralela sem especificação de threads
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }

    gettimeofday(&end, NULL);
    double tempo = time_diff(start, end);

    free(a); free(b); free(c);
    return tempo;
}

double calculos_intensivos() {
    double result = 0.0;
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Cálculo paralelo com variável compartilhada (cuidado com race condition)
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        result += sin(i) * cos(i);
    }

    gettimeofday(&end, NULL);
    return time_diff(start, end);
}

int main() {
    int threads_list[] = {1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    int num_tests = sizeof(threads_list) / sizeof(threads_list[0]);
    
    FILE *fp = fopen("dados_desempenho.csv", "w");
    if (fp == NULL) {
        printf("Erro ao criar arquivo CSV!\n");
        return 1;
    }
    
    fprintf(fp, "Threads,Tempo_Memoria,Tempo_CPU\n");

    for (int i = 0; i < num_tests; i++) {
        omp_set_num_threads(threads_list[i]);
        
        double tempo_mem = soma_vetores();
        double tempo_cpu = calculos_intensivos();
        
        fprintf(fp, "%d,%.6f,%.6f\n", threads_list[i], tempo_mem, tempo_cpu);
        printf("Threads: %2d | Memoria: %.4f s | CPU: %.4f s\n", 
               threads_list[i], tempo_mem, tempo_cpu);
    }
    
    fclose(fp);
    printf("\nDados salvos em 'dados_desempenho.csv'\n");
    
    return 0;
}