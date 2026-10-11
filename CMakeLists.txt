cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(RandomWalkSimulator VERSION 1.0 LANGUAGES CXX)

# Exigir el estándar C++17 para un mejor manejo de números aleatorios y estructuras
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Buscar e incluir OpenMP para el paralelismo
find_package(OpenMP REQUIRED)

# Definir el ejecutable y sus archivos fuente
add_executable(random_walk_sim
    src/main.cpp
    src/random_walk_1d.cpp
    src/random_walk_2d.cpp
)

# Vincular OpenMP al ejecutable
target_link_libraries(random_walk_sim PUBLIC OpenMP::OpenMP_CXX)

# Opciones adicionales de optimización para máximo rendimiento (O3)
target_compile_options(random_walk_sim PRIVATE -O3 -Wall -Wextra)