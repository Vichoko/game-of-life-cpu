# Game of life : CPU
Game of Life in Cs, displayed using C++ and OpenGL.

Detalle de implementación y experimentos en ```../informe.md```.

![runtime image](https://users.dcc.uchile.cl/~voyanede/cc7515/cpu_gol.png)


## Dependencias
* GL/glew.h
* SDL.h

## Compilación
```
$ make
```

Limpiar con;
```
$ make clean
```

## Instalar dependencias (Linux)
Si tira errores al compilar:
**Instalar librerías**
```
// GLEW, SDL2 (+SDL2_Image)

sudo apt-get install libglew-dev libsdl2-dev libsdl2-image-dev  # Debian, Ubuntu
sudo dnf install glew-devel SDL2-devel SDL2_image-devel # fedora
sudo pacman -S glew sdl2 sdl2_image # arch
```


**Asegurar tener compilador**:
```
sudo apt-get install build-essential libgl1-mesa-dev  # Debian, Ubuntu
sudo dnf install make gcc-c++  # Fedora
```


## Ejecución
```
$ ./cpu
```
Al precionar ```CTRL+C``` la aplicación terinará y mostrará estadísticas de cantidad de celulas evaluadas por segundo, y más.

## Customización
Se pueden modificar constantes en "src/globals.h".
```
#define WIDTH 800
#define HEIGHT 800
#define ROWS 128
#define COLUMNS 128
#define FRAMERATE 150
#define INITIAL_LIVES_FRACTION 0.9
#define THREADS_PER_BLOCK 8
```

### Resolución de vidas
Se pueden variar la cantidad de columnas (```COLUMNS```) y filas (```ROWS```).
### Cantidad inicial de vidas
Se puede variar el porcentaje de casillas inicial con vidas (```INITIAL_LIVES_FRACTION```).
### Frame rate
Se puede variar la cantidad de fotogramas por segundo máxima (```FRAMERATE```).
