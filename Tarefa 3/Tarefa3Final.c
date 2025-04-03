#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define INTERACOES 100000000

double calcular_pi(int loop) {
    double pi_aproximado = 0.0;
    for (int i = 0; i < loop; i++) {
        double termo = 1.0 / (2 * i + 1);
        pi_aproximado += (i % 2 == 0) ? termo : -termo;
    }
    return 4 * pi_aproximado;
}

int contar_digitos_corretos(double aproximado, double real) {
    char str_aprox[20], str_real[20];
    snprintf(str_aprox, sizeof(str_aprox), "%.15f", aproximado);
    snprintf(str_real, sizeof(str_real), "%.15f", real);

    int digitos_corretos = 0;
    for (int i = 0; str_aprox[i] != '\0' && str_real[i] != '\0'; i++) {
        if (str_aprox[i] == str_real[i] && str_aprox[i] != '.') {
            digitos_corretos++;
        } else if (str_aprox[i] != str_real[i]) {
            break;
        }
    }
    return digitos_corretos;
}

int main() {
    int iteracoes[] = {10, 50, 100, 200, 400, 800, 1000, 2000, 4000, 8000, 
                      10000, 20000, 40000, 60000, 80000, 100000, 200000, 
                      400000, 800000, 1000000, INTERACOES};

    int num_testes = sizeof(iteracoes) / sizeof(iteracoes[0]); //Calcula quantos elementos existem no array

    printf("Aproximacao de pi via Serie de Leibniz:\n");
    printf("Valor real de pi: %.15f\n\n", M_PI);

    for (int i = 0; i < num_testes; i++) {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        double pi_aproximado = calcular_pi(iteracoes[i]);

        clock_gettime(CLOCK_MONOTONIC, &end);
        
        // Calcula tempo em segundos com precisão de nanossegundos
        double tempo_execucao = (end.tv_sec - start.tv_sec) + 
                              (end.tv_nsec - start.tv_nsec) / 1e9;

        int digitos_corretos = contar_digitos_corretos(pi_aproximado, M_PI);

        printf("Iteracoes: %9d | Aproximacao: %.15f\n", iteracoes[i], pi_aproximado);
        printf("Digitos corretos: %d\n", digitos_corretos);
        printf("Tempo: %.9f segundos\n\n", tempo_execucao);
    }

    return 0;
}