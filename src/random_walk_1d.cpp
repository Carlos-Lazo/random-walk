#include "random_walk_1d.h"
#include "checkpoints.h"
#include <iostream>
#include <vector>
#include <random>
#include <omp.h>
#include <fstream>
#include <iomanip>
#include <cmath>

void simulate_1d(long N, long M) {
    // El tamaño máximo del histograma es 2N + 1. El índice N corresponde a la posición 0.
    std::vector<long> histogram(2 * N + 1, 0);
    double total_distance = 0.0;

    // Pasos en los que se mide la distancia para la curva <d(n)> vs n
    const std::vector<long> checkpoints = make_checkpoints(N);
    const std::size_t K = checkpoints.size();
    // Suma global (sobre las M caminatas) de la distancia en cada checkpoint
    std::vector<double> dist_sum(K, 0.0);

    // Abrir bloque paralelo y declarar la variable de reducción para la distancia
    #pragma omp parallel reduction(+:total_distance)
    {
        // 1. Aislamiento del PRNG: Cada hilo inicializa su propio motor Mersenne Twister
        std::random_device rd;
        // Se aplica un XOR entre un valor aleatorio del hardware y el ID del hilo para garantizar semillas únicas
        std::mt19937 gen(rd() ^ omp_get_thread_num());
        std::uniform_int_distribution<> dis(0, 1);

        // Acumulador privado de la distancia por checkpoint (se fusiona una sola vez al final)
        std::vector<double> local_dist(K, 0.0);

        // 2. Distribución de carga: OpenMP divide las M caminatas entre los hilos activos
        #pragma omp for
        for (long i = 0; i < M; ++i) {
            long position = 0;
            long step = 0;

            // Simular los N pasos, deteniéndose en cada checkpoint para registrar |posicion|
            for (std::size_t k = 0; k < K; ++k) {
                for (; step < checkpoints[k]; ++step) {
                    position += (dis(gen) == 1) ? 1 : -1;
                }
                local_dist[k] += std::abs(position);
            }

            // Acumular la distancia absoluta final para la reducción global
            total_distance += std::abs(position);

            // 3. Operación atómica: Garantiza que la suma al histograma compartido sea Thread-Safe
            #pragma omp atomic
            histogram[position + N]++;
        }

        // Fusión de los acumuladores locales en el global (una vez por hilo)
        #pragma omp critical
        {
            for (std::size_t k = 0; k < K; ++k) {
                dist_sum[k] += local_dist[k];
            }
        }
    }

    // 4. Exportación del histograma de probabilidad
    std::ofstream outfile("data/resultados_1d.csv");
    if (outfile.is_open()) {
        outfile << std::setprecision(10);
        outfile << "Posicion,Frecuencia,Probabilidad,ProbTeorica\n";
        for (long i = 0; i < 2 * N + 1; ++i) {
            if (histogram[i] > 0) {
                // Se resta N para revertir el desplazamiento y recuperar la posición real
                long pos = i - N;
                // Probabilidad empirica = frecuencia / M (la suma de todas vale 1)
                double prob = static_cast<double>(histogram[i]) / M;
                // Probabilidad teorica exacta: P(S_N = k) = C(N, (N+k)/2) / 2^N  (distribución binomial).
                // Solo existen posiciones con la misma paridad que N. Se calcula con lgamma para evitar desbordes.
                double k_der = (N + pos) / 2.0;
                double k_izq = (N - pos) / 2.0;
                double prob_teo = std::exp(std::lgamma(N + 1.0) - std::lgamma(k_der + 1.0)
                                           - std::lgamma(k_izq + 1.0) - N * std::log(2.0));
                outfile << pos << "," << histogram[i] << "," << prob << "," << prob_teo << "\n";
            }
        }
        outfile.close();
    } else {
        std::cerr << "Error: No se pudo abrir data/resultados_1d.csv para escritura. Asegúrese de que la carpeta exista.\n";
    }

    // 5. Exportación de la distancia promedio en función de los pasos
    std::ofstream distfile("data/distancia_1d.csv");
    if (distfile.is_open()) {
        distfile << std::setprecision(10);
        distfile << "Paso,DistPromedio,DistTeorica\n";
        for (std::size_t k = 0; k < K; ++k) {
            long n = checkpoints[k];
            // La media teórica asintótica de |S_n| en 1D es sqrt(2n / pi)
            distfile << n << "," << dist_sum[k] / M << "," << std::sqrt(2.0 * n / M_PI) << "\n";
        }
        distfile.close();
    } else {
        std::cerr << "Error: No se pudo abrir data/distancia_1d.csv para escritura. Asegúrese de que la carpeta exista.\n";
    }

    // 6. Verificación del Teorema
    double emp_dist = total_distance / M;
    // La media teórica del valor absoluto en una caminata 1D asintóticamente es sqrt(2N / pi)
    double theo_dist = std::sqrt(2.0 * N / M_PI);

    std::cout << "--- Resultados 1D ---\n";
    std::cout << "Distancia promedio empirica: " << emp_dist << "\n";
    std::cout << "Distancia promedio teorica:  " << theo_dist << "\n";
    std::cout << "Datos exportados a data/resultados_1d.csv y data/distancia_1d.csv\n";
}
