#ifndef CHECKPOINTS_H
#define CHECKPOINTS_H

#include <vector>
#include <cmath>

// Devuelve la lista (estrictamente creciente) de pasos n en los que se mide la
// distancia al origen para construir la curva <d(n)> vs n.
//
//  - Si N <= max_puntos se mide en TODOS los pasos 1, 2, ..., N.
//  - Si N > max_puntos se usan ~max_puntos pasos espaciados en escala
//    logaritmica entre 1 y N (se ven bien en un grafico log-log y evitan
//    calcular una raiz cuadrada en cada uno de los N pasos de cada caminata).
//
// El ultimo elemento es siempre N.
inline std::vector<long> make_checkpoints(long N, long max_puntos = 300) {
    std::vector<long> cps;
    if (N <= 0) return cps;

    if (N <= max_puntos) {
        cps.reserve(N);
        for (long n = 1; n <= N; ++n) cps.push_back(n);
        return cps;
    }

    long last = 0;
    for (long i = 0; i < max_puntos; ++i) {
        double frac = static_cast<double>(i) / (max_puntos - 1);
        long n = std::lround(std::pow(static_cast<double>(N), frac));
        if (n < 1) n = 1;
        if (n > N) n = N;
        if (n > last) {          // descarta repetidos (al principio la escala log da enteros iguales)
            cps.push_back(n);
            last = n;
        }
    }
    if (cps.back() != N) cps.push_back(N);
    return cps;
}

#endif
