import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import os

base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
data_path = os.path.join(base_dir, 'data', 'resultados_2d.csv')
output_path = os.path.join(base_dir, 'data', 'heatmap_2d.png')


try:
    df = pd.read_csv(data_path)
except FileNotFoundError:
    print(f"Error: No se encontró el archivo {data_path}. Ejecute la simulación 2D primero.")
    exit(1)

if 'Probabilidad' not in df.columns:
    print("Error: el CSV no tiene la columna 'Probabilidad'. Vuelva a ejecutar la simulación 2D con la versión actual.")
    exit(1)

# Forzar el uso del backend interactivo de PyQt5
matplotlib.use('Qt5Agg')

plt.figure(figsize=(9, 7))
# El tamaño del marcador (s) puede ajustarse dependiendo de N. 'inferno' resalta densidades altas.
scatter = plt.scatter(df['X'], df['Y'], c=df['Probabilidad'], cmap='inferno', marker='s', s=8)

cbar = plt.colorbar(scatter)
cbar.set_label('Probabilidad de Llegada', rotation=270, labelpad=15)

plt.title('Mapa de Calor Espacial (Random Walk 2D)', fontsize=14)
plt.xlabel('Eje X', fontsize=12)
plt.ylabel('Eje Y', fontsize=12)

# Dibujar los ejes centrales para referencia espacial
plt.axhline(0, color='white', linewidth=0.5, linestyle='--')
plt.axvline(0, color='white', linewidth=0.5, linestyle='--')

# Fondo oscuro para evidenciar la dispersión
plt.gca().set_facecolor('#1c1c1c')

plt.tight_layout()
plt.savefig(output_path, dpi=300)
print(f"Gráfico guardado en: {output_path}")
plt.show()
