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

Este script leerá `data/resultados_1d.csv` y mostrará una ventana interactiva con el gráfico de barras. Al cerrarla, guardará automáticamente una imagen de alta resolución en `data/histograma_1d.png`.

### Generar mapa de calor 2D

`python3 plot_heatmap.py`

Este script leerá `data/resultados_2d.csv` y renderizará un gráfico de dispersión espacial. El color de cada punto representará la frecuencia de visitas. Al finalizar, guardará la imagen en `data/heatmap_2d.png`.