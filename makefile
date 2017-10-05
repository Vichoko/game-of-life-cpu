CPPFLAGS=-I/usr/include/SDL2 -g
LDLIBS=-lSDL2 -lGLEW -lGL
all: CPU
clean:
	rm -f *.o CPU
CPU: shaders/shader_utils.o


