# Visualización de simulacion de random walking en c++

Este directorio contiene los scripts de visualización en Python diseñados para renderizar los resultados numéricos generados por el simulador en C++. 

## Entorno virtual

Para ejecutar los scripts se recomienda utilizar un entorno virtual de Python.

**Archivo de dependencias**: `requirements.txt`.

### Configurar el entorno virtual (Linux/macOS)

Dentro de `scripts/` ejecuta secuencialmente:

```bash 
# Crear el entorno virtual llamado 'venv'
python3 -m venv venv

# Activar el entorno virtual
source venv/bin/activate

# Instalar las dependencias requeridas
pip install -r requirements.txt
```


### Configurar el entorno virtual (Windows / PowerShell)

```powershell
# Crear el entorno virtual llamado 'venv'
python -m venv venv

# Activar el entorno virtual
.\venv\scripts\Activate.ps1

# Instalar las dependencias requeridas
pip install -r requirements.txt
```

(Nota: Para salir del entorno virtual en cualquier sistema operativo, simplemente ejecuta el comando `deactivate`).

## Ejecución de los scripts de visualización

**Importante**: 
- Asegúrate de haber ejecutado previamente los binarios generados en la Simulacion de random walking en c++ para que los archivos `data/resultados_1d.csv` y `data/resultados_2d.csv` existan.

Ejecuta los siguientes comandos:

### Generar Histograma 1D

`python3 plot_histogram.py`

Este script leerá `data/resultados_1d.csv` y mostrará una ventana interactiva con el histograma de probabilidad (frecuencia / M) y la curva binomial teórica superpuesta. Al cerrarla, guardará automáticamente una imagen de alta resolución en `data/histograma_1d.png`.

### Generar mapa de calor 2D

`python3 plot_heatmap.py`

Este script leerá `data/resultados_2d.csv` y renderizará un gráfico de dispersión espacial. El color de cada punto representará la probabilidad de llegada (frecuencia / M). Al finalizar, guardará la imagen en `data/heatmap_2d.png`.

### Generar la curva distancia promedio vs. pasos

`python3 plot_distancia.py`

Este script leerá `data/distancia_1d.csv` y/o `data/distancia_2d.csv` (los que existan) y graficará, en escala log-log, la distancia promedio al origen en función del número de pasos junto con la curva teórica (`sqrt(2n/π)` en 1D y `sqrt(πn)/2` en 2D). Guardará la imagen en `data/distancia_vs_pasos.png`.

**Nota**: los CSV generados con versiones anteriores del simulador no tienen la columna `Probabilidad`; hay que volver a ejecutar la simulación.