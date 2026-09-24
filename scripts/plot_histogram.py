import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import os

# Asegurar que el script se ejecute correctamente independientemente del directorio actual
base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
data_path = os.path.join(base_dir, 'data', 'resultados_1d.csv')
output_path = os.path.join(base_dir, 'data', 'histograma_1d.png')

try:
    df = pd.read_csv(data_path)
except FileNotFoundError:
    print(f"Error: No se encontró el archivo {data_path}. Ejecute la simulación 1D primero.")
    exit(1)

# Forzar el uso del backend interactivo de PyQt5
matplotlib.use('Qt5Agg')

plt.figure(figsize=(10, 6))
# Se utiliza un gráfico de barras relleno para representar el histograma discreto
plt.bar(df['Posicion'], df['Frecuencia'], width=1.0, color='royalblue', alpha=0.8)

plt.title('Distribución de Posiciones Finales (Random Walk 1D)', fontsize=14)
plt.xlabel('Posición Final respecto al Origen', fontsize=12)
plt.ylabel('Cantidad de Individuos (Frecuencia)', fontsize=12)
plt.grid(axis='y', linestyle='--', alpha=0.7)

plt.tight_layout()
plt.savefig(output_path, dpi=300)
print(f"Gráfico guardado en: {output_path}")
plt.show()