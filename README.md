# Simulacion de random walking en c++

Este repositorio contiene el código fuente y las herramientas de análisis para una Simulacion de random walking en c++. El software utiliza C++ y OpenMP para simular millones de caminatas aleatorias (Random Walks) en 1D y 2D de manera paralela.

## Estructura del directorio

La arquitectura está diseñada para separar el cálculo numérico intensivo (backend en C++) del análisis visual de datos (frontend en Python).

* **CMakeLists.txt**: Archivo de configuración central que orquesta la compilación del proyecto y garantiza el enlace correcto con la librería OpenMP.

* **src/**: Contiene todo el código fuente.

    * **main.cpp**: Controlador principal que recibe parámetros por consola, inicializa los temporizadores e invoca la simulación correspondiente.

    * **random_walk_1d.** **cpp / .h**: Implementación de la caminata en una dimensión, utilizando un arreglo desplazado para la recolección atómica de datos paralelos.

    * **random_walk_2d.** **cpp / .h**: Implementación de la caminata en dos dimensiones, gestionando Hash Maps locales por hilo para gestionar los límites de memoria.

    * **checkpoints.h**: Función auxiliar que define en qué pasos se mide la distancia al origen para la curva distancia promedio vs. pasos (todos los pasos si N ≤ 300; ~300 pasos en escala logarítmica si N es mayor).

* **scripts/**: Entorno de visualización.

    * **plot_histogram.py**: Lee los datos de C++ y grafica el histograma de **probabilidad** de la posición final en 1D, junto con la distribución binomial teórica.

    * **plot_heatmap.py**: Lee los datos de C++ y genera un gráfico de dispersión bidimensional coloreado por **probabilidad** de llegada (mapa de calor).

    * **plot_distancia.py**: Grafica la distancia promedio al origen en función de la cantidad de pasos (simulación vs. teoría, escala log-log).

* **data/**: Directorio de intercambio. La Simulacion deposita aquí los archivos resultados_1d.csv, resultados_2d.csv (histogramas con columnas de frecuencia y probabilidad) y distancia_1d.csv, distancia_2d.csv (distancia promedio vs. pasos). Posteriormente, los scripts de Python guardan aquí las imágenes generadas (**.png**).

* **build/**: Carpeta de trabajo para el sistema de construcción. Aislada del código fuente, almacena los archivos objeto intermedios y el ejecutable binario resultante.

* **docs/**: Repositorio de documentación donde se registrarán las tablas de tiempos, la gráfica de cálculo del Speedup y las conclusiones formales sobre la eficiencia paralela.



## Configuración por defecto

Al ejecutar el programa sin proporcionar parámetros, se activará una configuración diseñada para ofrecer resultados estadísticos válidos en una fracción de segundo en cualquier PC de uso general:

*   **Dimensión:** `2` (Simula un plano cartesiano, que es el caso de uso más representativo).
*   **Pasos por caminata ($N$):** `1000` (Suficientes para observar el fenómeno de difusión y alejamiento del origen).
*   **Número de simulaciones ($M$):** `100000` (Garantiza una muestra estadística lo bastante grande para que la campana de Gauss tome forma, sin consumir demasiada RAM).
*   **Hilos de ejecución ($T$):** Automático. Detecta el máximo disponible en el procesador local.

## Ejemplos de Uso

### 1. Compilación
Asegúrate de tener instalados estos paquetes (Linux):
```bash
sudo apt-get update
sudo apt-get install libomp-dev libgomp1 build-essential
```

Dentro de `build/`:

``` bash
cmake ..
make
```

### 2. Ejecución Básica
Desde el directorio raiz ejecutando con configuración por defecto:
```bash
./build/random_walk_sim
```
<br>
Salida esperada:

```bash
Ejecutando con configuración por defecto...
Simulacion 2D: N=1000, M=100000, Hilos=8
Tiempo de ejecucion: 0.145 segundos.
```

### Ejecución Personalizada
La sintaxis exige 4 parámetros: `<DIMENSION>` `<PASOS>` `<CAMINATAS>` `<HILOS>`

**Ejemplo A**: Validación matemática en 1D:
Para correr 10 millones de simulaciones cortas en 1 dimensión usando 4 hilos.

```bash
./random_walk_sim 1 100 10000000 4
```

**Ejemplo B**: Prueba de Estrés para el Servidor (Speedup):
Para estresar un servidor o una PC de alto rendimiento, puedes usar 50.000 pasos en un millón de simulaciones, forzando a todos los hilos:

```bash
./random_walk_sim 2 50000 1000000 16
```

## Benchmark

``run_benchmark.sh``<p>Ejecutará el binario random_walk_sim iterativamente incrementando los hilos, tomará las mediciones, calculará la mediana (para descartar ruido del sistema) y exportará la tabla final directamente a un archivo de texto.<p>

<p>El script utiliza *bc* (la calculadora por consola de Linux) para las divisiones decimales, y un minúsculo script de python3 para calcular matemáticamente la mediana de las $7$ ejecuciones. Asegúrate de tener *bc* instalado (`sudo apt install bc`)<p>
<p>El script realiza un total de 35 ejecuciones (5 configuraciones de hilos × 7 repeticiones)<p>

### Configuración del Benchmark
```BASH
DIM=2 #Dimension
N=50000 #Pasos
M=1000000 #Caminatas
```
### Lista de hilos a evaluar (ajusta esto según los núcleos físicos/lógicos de tu PC)

```BASH
THREADS_LIST = (1 2 4 8 16)
```
### Número de ejecuciones por configuración para calcular la mediana

```BASH
RUNS = 7
```

### Archivos de salida
```
OUTPUT_FILE = "docs/speedup_resultados.txt"
```

### Correr el script

```bash
./run_benchmark.sh
```