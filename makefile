CPPFLAGS=-I/usr/include/SDL2 -g
LDLIBS=-lSDL2 -lGLEW -lGL
all: cpu
clean:
	rm -f *.o cpu
	cd shaders
	rm -f *.o
cpu : shader_utils.o


