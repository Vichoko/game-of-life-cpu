# Game of life : CPU
## Dependencias
* GL/glew.h
* SDL.h

## Compilación
```
$ cd ./src
$ make
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
$ ./CPU
```

## Customización
### Resolución de vidas
Se pueden variar la cantidad de columnas (```COLUMNS```) y filas (```ROWS```).
### Cantidad inicial de vidas
Se puede variar el porcentaje de casillas inicial con vidas (```INITIAL_LIVES_FRACTION```).
### Frame rate
Se puede variar la cantidad de fotogramas por segundo máxima (```FRAMERATE```).
