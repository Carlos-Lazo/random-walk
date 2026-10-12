#!/bin/bash

# --- Configuración del Benchmark ---
DIM=2
N=50000
M=1000000
# Lista de hilos a evaluar (ajusta esto según los núcleos físicos/lógicos de tu PC)
THREADS_LIST=(1 2 4 8 16)
# Número de ejecuciones por configuración para calcular la mediana
RUNS=7

# Archivos de salida
OUTPUT_FILE="docs/speedup_resultados.txt"
TMP_TIMES="tmp_times.txt"

# Crear directorio docs si no existe
mkdir -p docs

# --- Inicialización del Archivo de Salida ---
echo "==========================================" > $OUTPUT_FILE
echo "   REPORTE DE RENDIMIENTO (SPEEDUP)       " >> $OUTPUT_FILE
echo "==========================================" >> $OUTPUT_FILE
echo "Carga de Trabajo: Dimensión $DIM | N=$N | M=$M" >> $OUTPUT_FILE
echo "Ejecuciones por configuración: $RUNS" >> $OUTPUT_FILE
echo "------------------------------------------" >> $OUTPUT_FILE
printf "%-10s | %-15s | %-10s | %-15s\n" "Hilos (p)" "Tiempo (seg)" "Speedup" "Eficiencia" >> $OUTPUT_FILE
echo "------------------------------------------" >> $OUTPUT_FILE
echo "Los resultados se guardarán en $OUTPUT_FILE"

echo "Iniciando benchmark"

# Variable para almacenar el tiempo base (T1)
TIEMPO_BASE=0

# --- Bucle Principal ---
for p in "${THREADS_LIST[@]}"; do
    echo ">> Evaluando con $p hilo(s)..."
    rm -f $TMP_TIMES
    
    # Ejecutar la simulación RUNS veces
    for ((i=1; i<=RUNS; i++)); do
        # Ejecutamos el programa y extraemos solo la línea del tiempo
        # Usamos grep y awk para parsear el número de segundos
        TIEMPO=$(./build/random_walk_sim $DIM $N $M $p | grep "Tiempo de ejecucion:" | awk '{print $4}')
        echo "$TIEMPO" >> $TMP_TIMES
    done

    # --- Calcular la Mediana usando Python (por simplicidad y precisión) ---
    MEDIANA=$(python3 -c "
import sys, statistics
tiempos = [float(line.strip()) for line in sys.stdin if line.strip()]
print(f'{statistics.median(tiempos):.4f}')
" < $TMP_TIMES)

    # --- Calcular Speedup y Eficiencia ---
    if [ "$p" -eq 1 ]; then
        TIEMPO_BASE=$MEDIANA
        SPEEDUP="1.0000"
        EFICIENCIA="1.0000"
    else
        # Usamos 'bc' para realizar la división con decimales en Bash
        SPEEDUP=$(echo "scale=4; $TIEMPO_BASE / $MEDIANA" | bc)
        EFICIENCIA=$(echo "scale=4; $SPEEDUP / $p" | bc)
    fi

    # Imprimir en consola y guardar en el archivo
    printf "   Mediana: %s seg | Speedup: %s | Eficiencia: %s\n" "$MEDIANA" "$SPEEDUP" "$EFICIENCIA"
    printf "%-10s | %-15s | %-10s | %-15s\n" "$p" "$MEDIANA" "$SPEEDUP" "$EFICIENCIA" >> $OUTPUT_FILE

done

# Limpiar archivo temporal
rm -f $TMP_TIMES

echo "------------------------------------------" >> $OUTPUT_FILE
echo "Benchmark completado exitosamente."
echo "Resultados exportados a $OUTPUT_FILE"