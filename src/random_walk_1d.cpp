#include "random_walk_1d.h"
#include <iostream>
#include <vector>
#include <random>
#include <omp.h>
#include <fstream>
#include <cmath>

void simulate_1d(long N, long M) {
    // El tamaño máximo del histograma es 2N + 1. El índice N corresponde a la posición 0.
    std::vector<long> histogram(2 * N + 1, 0);
    double total_distance = 0.0;

    // Abrir bloque paralelo y declarar la variable de reducción para la distancia
    #pragma omp parallel reduction(+:total_distance)
    {
        // 1. Aislamiento del PRNG: Cada hilo inicializa su propio motor Mersenne Twister
        std::random_device rd;
        // Se aplica un XOR entre un valor aleatorio del hardware y el ID del hilo para garantizar semillas únicas
        std::mt19937 gen(rd() ^ omp_get_thread_num());
        std::uniform_int_distribution<> dis(0, 1);

        // 2. Distribución de carga: OpenMP divide las M caminatas entre los hilos activos
        #pragma omp for
        for (long i = 0; i < M; ++i) {
            long position = 0;
            
            // Simular los N pasos del individuo actual
            for (long j = 0; j < N; ++j) {
                position += (dis(gen) == 1) ? 1 : -1;
            }

            // Acumular la distancia absoluta para la reducción global
            total_distance += std::abs(position);

            // 3. Operación atómica: Garantiza que la suma al histograma compartido sea Thread-Safe
            #pragma omp atomic
            histogram[position + N]++;
        }
    }

    // 4. Exportación de datos
    std::ofstream outfile("data/resultados_1d.csv");
    if (outfile.is_open()) {
        outfile << "Posicion,Frecuencia\n";
        for (long i = 0; i < 2 * N + 1; ++i) {
            if (histogram[i] > 0) {
                // Se resta N para revertir el desplazamiento y recuperar la posición real
                outfile << (i - N) << "," << histogram[i] << "\n";
            }
        }
        outfile.close();
    } else {
        std::cerr << "Error: No se pudo abrir data/resultados_1d.csv para escritura. Asegúrese de que la carpeta exista.\n";
    }

    // 5. Verificación del Teorema
    double emp_dist = total_distance / M;
    // La media teórica del valor absoluto en una caminata 1D asintóticamente es sqrt(2N / pi)
    double theo_dist = std::sqrt(2.0 * N / M_PI);

    std::cout << "--- Resultados 1D ---\n";
    std::cout << "Distancia promedio empirica: " << emp_dist << "\n";
    std::cout << "Distancia promedio teorica:  " << theo_dist << "\n";
    std::cout << "Datos exportados a data/resultados_1d.csv\n";
}