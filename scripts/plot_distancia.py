import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import os

base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
data_dir = os.path.join(base_dir, 'data')
output_path = os.path.join(data_dir, 'distancia_vs_pasos.png')

# Se grafica cada dimensión para la que exista su CSV
paneles = []
for dim, titulo in ((1, 'Random Walk 1D'), (2, 'Random Walk 2D')):
    ruta = os.path.join(data_dir, f'distancia_{dim}d.csv')
    if os.path.exists(ruta):
        paneles.append((titulo, pd.read_csv(ruta)))

if not paneles:
    print(f"Error: no se encontró ningún archivo distancia_*.csv en {data_dir}. Ejecute la simulación primero.")
    exit(1)

# Backend interactivo de PyQt5; si no está disponible se guarda solo la imagen
try:
    matplotlib.use('Qt5Agg')
    interactivo = True
except ImportError:
    matplotlib.use('Agg')
    interactivo = False

fig, axes = plt.subplots(1, len(paneles), figsize=(6.5 * len(paneles), 5.5), squeeze=False)

for ax, (titulo, df) in zip(axes[0], paneles):
    ax.plot(df['Paso'], df['DistPromedio'], color='royalblue', linewidth=2.2, label='Simulación')
    ax.plot(df['Paso'], df['DistTeorica'], color='crimson', linewidth=1.6, linestyle='--', label='Teoría (asintótica)')
    # Escala log-log: la ley de difusión <d> ~ sqrt(n) se ve como una recta de pendiente 1/2
    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_title(titulo, fontsize=13)
    ax.set_xlabel('Cantidad de pasos n', fontsize=12)
    ax.set_ylabel('Distancia promedio al origen <d(n)>', fontsize=12)
    ax.grid(True, which='both', linestyle='--', alpha=0.5)
    ax.legend()

fig.suptitle('Distancia promedio en función de los pasos', fontsize=14)
fig.tight_layout()
fig.savefig(output_path, dpi=300)
print(f"Gráfico guardado en: {output_path}")
if interactivo:
    plt.show()
