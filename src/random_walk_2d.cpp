#include "random_walk_2d.h"
#include <iostream>
#include <vector>
#include <random>
#include <omp.h>
#include <fstream>
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

    #pragma omp parallel reduction(+:total_distance)
    {
        std::random_device rd;
        std::mt19937 gen(rd() ^ omp_get_thread_num());
        std::uniform_int_distribution<> dis(0, 3);
        
        // Estado Local por Hilo: Evita colisiones de memoria (Thread-Safety)
        std::unordered_map<std::pair<long, long>, long, PairHash> local_histogram;

        // Arreglos de desplazamiento rápido: Derecha, Izquierda, Arriba, Abajo
        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

        #pragma omp for
        for (long i = 0; i < M; ++i) {
            long x = 0;
            long y = 0;
            
            for (long j = 0; j < N; ++j) {
                int dir = dis(gen);
                x += dx[dir];
                y += dy[dir];
            }

            // Suma de distancias euclidianas
            total_distance += std::sqrt(x * x + y * y);
            
            // Registrar posición en el mapa local del hilo
            local_histogram[{x, y}]++;
        }

        // Fusión secuencial segura: Los hilos transfieren sus datos al mapa global
        #pragma omp critical
        {
            for (const auto& pair : local_histogram) {
                global_histogram[pair.first] += pair.second;
            }
        }
    }

    std::ofstream outfile("data/resultados_2d.csv");
    if (outfile.is_open()) {
        outfile << "X,Y,Frecuencia\n"; // Cabecera
        for (const auto& pair : global_histogram) {
            outfile << pair.first.first << "," << pair.first.second << "," << pair.second << "\n";
        }
        outfile.close();
    } else {
        std::cerr << "Error al abrir data/resultados_2d.csv\n";
    }

    double emp_dist = total_distance / M;
    // Distancia euclidiana media esperada para el modelo 2D estocástico
    double theo_dist = std::sqrt(M_PI * N) / 2.0;

    std::cout << "--- Resultados 2D ---\n";
    std::cout << "Distancia promedio empirica: " << emp_dist << "\n";
    std::cout << "Distancia promedio teorica:  " << theo_dist << "\n";
    std::cout << "Datos exportados a data/resultados_2d.csv\n";
}