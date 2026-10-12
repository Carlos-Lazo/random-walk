#include "random_walk_2d.h"
#include "checkpoints.h"
#include <iostream>
#include <vector>
#include <random>
#include <omp.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <unordered_map>

// Definición de función Hash para utilizar pares de coordenadas (X, Y)
struct PairHash {
    std::size_t operator()(const std::pair<long, long>& p) const {
        return std::hash<long>()(p.first) ^ (std::hash<long>()(p.second) << 1);
    }
};

void simulate_2d(long N, long M) {
    // Mapa global disperso para almacenar la posición final de todas las caminatas
    std::unordered_map<std::pair<long, long>, long, PairHash> global_histogram;
    double total_distance = 0.0;

    // Pasos en los que se mide la distancia para la curva <d(n)> vs n
    // (la raiz cuadrada se calcula solo en estos puntos, no en los N pasos)
    const std::vector<long> checkpoints = make_checkpoints(N);
    const std::size_t K = checkpoints.size();
    // Suma global (sobre las M caminatas) de la distancia euclidiana en cada checkpoint
    std::vector<double> dist_sum(K, 0.0);

    #pragma omp parallel reduction(+:total_distance)
    {
        std::random_device rd;
        std::mt19937 gen(rd() ^ omp_get_thread_num());
        std::uniform_int_distribution<> dis(0, 3);

        // Estado Local por Hilo: Evita colisiones de memoria (Thread-Safety)
        std::unordered_map<std::pair<long, long>, long, PairHash> local_histogram;
        std::vector<double> local_dist(K, 0.0);

        // Arreglos de desplazamiento rápido: Derecha, Izquierda, Arriba, Abajo
        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

        #pragma omp for
        for (long i = 0; i < M; ++i) {
            long x = 0;
            long y = 0;
            long step = 0;

            // Simular los N pasos, deteniéndose en cada checkpoint para registrar la distancia
            for (std::size_t k = 0; k < K; ++k) {
                for (; step < checkpoints[k]; ++step) {
                    int dir = dis(gen);
                    x += dx[dir];
                    y += dy[dir];
                }
                local_dist[k] += std::sqrt(static_cast<double>(x) * x + static_cast<double>(y) * y);
            }

            // Suma de distancias euclidianas finales
            total_distance += std::sqrt(static_cast<double>(x) * x + static_cast<double>(y) * y);

            // Registrar posición en el mapa local del hilo
            local_histogram[{x, y}]++;
        }

        // Fusión secuencial segura: Los hilos transfieren sus datos al mapa global
        #pragma omp critical
        {
            for (const auto& pair : local_histogram) {
                global_histogram[pair.first] += pair.second;
            }
            for (std::size_t k = 0; k < K; ++k) {
                dist_sum[k] += local_dist[k];
            }
        }
    }

    // Histograma de probabilidad: Probabilidad = Frecuencia / M (la suma de todas vale 1)
    std::ofstream outfile("data/resultados_2d.csv");
    if (outfile.is_open()) {
        outfile << std::setprecision(10);
        outfile << "X,Y,Frecuencia,Probabilidad\n"; // Cabecera
        for (const auto& pair : global_histogram) {
            outfile << pair.first.first << "," << pair.first.second << "," << pair.second << ","
                    << static_cast<double>(pair.second) / M << "\n";
        }
        outfile.close();
    } else {
        std::cerr << "Error al abrir data/resultados_2d.csv\n";
    }

    // Distancia promedio en función de los pasos
    std::ofstream distfile("data/distancia_2d.csv");
    if (distfile.is_open()) {
        distfile << std::setprecision(10);
        distfile << "Paso,DistPromedio,DistTeorica\n";
        for (std::size_t k = 0; k < K; ++k) {
            long n = checkpoints[k];
            // Distancia euclidiana media esperada (asintótica) para el modelo 2D: sqrt(pi*n)/2
            distfile << n << "," << dist_sum[k] / M << "," << std::sqrt(M_PI * n) / 2.0 << "\n";
        }
        distfile.close();
    } else {
        std::cerr << "Error al abrir data/distancia_2d.csv\n";
    }

    double emp_dist = total_distance / M;
    // Distancia euclidiana media esperada para el modelo 2D estocástico
    double theo_dist = std::sqrt(M_PI * N) / 2.0;

    std::cout << "--- Resultados 2D ---\n";
    std::cout << "Distancia promedio empirica: " << emp_dist << "\n";
    std::cout << "Distancia promedio teorica:  " << theo_dist << "\n";
    std::cout << "Datos exportados a data/resultados_2d.csv y data/distancia_2d.csv\n";
}
