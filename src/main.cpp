#include <iostream>
#include <string>
#include <omp.h>
#include "random_walk_1d.h"
#include "random_walk_2d.h"

int main(int argc, char* argv[]) {
    // Configuracion por defecto
    int dim = 2;
    long N = 1000;
    long M = 100000;
    int num_threads = omp_get_max_threads();

    // Validacion de argumentos
    if (argc == 1) {
        std::cout << "Ejecutando con configuracion por defecto...\n";
    } else if (argc == 5) {
        dim = std::stoi(argv[1]);
        N = std::stol(argv[2]);
        M = std::stol(argv[3]);
        num_threads = std::stoi(argv[4]);
    } else {
        std::cerr << "Uso personalizado: " << argv[0] << " <dimension (1 o 2)> <pasos N> <caminatas M> <hilos T>\n";
        std::cerr << "Uso por defecto:   " << argv[0] << "\n";
        return 1;
    }

    omp_set_num_threads(num_threads);
    double start_time = omp_get_wtime();

    if (dim == 1) {
        std::cout << "Simulacion 1D: N=" << N << ", M=" << M << ", Hilos=" << num_threads << "\n";
        simulate_1d(N, M);
    } else if (dim == 2) {
        std::cout << "Simulacion 2D: N=" << N << ", M=" << M << ", Hilos=" << num_threads << "\n";
        simulate_2d(N, M);
    } else {
        std::cerr << "Error: La dimension debe ser 1 o 2.\n";
        return 1;
    }

    double end_time = omp_get_wtime();
    std::cout << "Tiempo de ejecucion: " << (end_time - start_time) << " segundos.\n";

    return 0;
}