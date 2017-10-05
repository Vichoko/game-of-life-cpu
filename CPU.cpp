//============================================================================
// Name        : CPU.cpp
// Author      : Vicente Oyanedel Muñoz
// Version     : 2.7
// Copyright   : Copyright (c) 2017
// Description : Game of life sequentially implemented, using C and OpenGL.
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <tgmath.h>
// display
#include <cstdlib>
#include <iostream>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using SDL2 for the base window and OpenGL context init */
#include <SDL.h>
#include "shaders/shader_utils.h"
#include "CPU.h"

#define ROWS 512
#define COLUMNS 256
#define FRAMERATE 150
#define INITIAL_LIVES_FRACTION 0.6
// En des-uso (ASCII drawing) #define BUFFER_SIZE 9000

// juego
int* livesArrayActual;
int* livesArrayNext;

// display
GLuint program;
GLuint vbo_triangle;
GLuint vao_triangle;
GLint attribute_coord2d;
GLint attribute_color;

// vertices + colores
container_t* vertex_n_colors;

int win_width = WIDTH;
int win_height = HEIGHT;

using namespace std;

/**
 * Mapea coordenadas x,y (comienzan de 0) a un arreglo uni-dimensional de celdas vivas (1) o muertas(0).
 */
int isCellAlive(int column, int row){
	return livesArrayActual[row*COLUMNS + column];
}
void setCellAlive(int column, int row){
	livesArrayNext[row*COLUMNS + column] = 1;
}
void setCellDead(int column, int row){
	livesArrayNext[row*COLUMNS + column] = 0;
}


/**
 * Cuenta los vecinos que esten vivos. Considera el ciclidad en bordes.
 *  * 
 * Retorna cantidad de vecinos vivos.
 * */
int countAliveNeighbors(int column, int row){
	int neighborColumn;
	int neighborRow;
	int aliveNeighbors = 0;
	for (int x = -1; x <= 1; x++){
		for (int y = -1; y <= 1; y++){
			if (x == 0 && y ==0) // itself bypass
				continue;
			neighborColumn = column+x;
			neighborRow = row+y;

			// edge conditions
			if (neighborColumn < 0){
				neighborColumn = COLUMNS-1;
			} else if (neighborColumn > COLUMNS-1){
				neighborColumn = 0;
			} if (neighborRow < 0){
				neighborRow = ROWS-1;
			} else if (neighborRow > ROWS-1){
				neighborRow = 0;
			}

			if (isCellAlive(neighborColumn, neighborRow)){
				aliveNeighbors++;
			}
		}
	}
	return aliveNeighbors;
}

/**
 * Dado una coordenada del juego de la vida, se calcula si la celda estara viva o muerta en la siguiente iteracion.
 */

void checkLife(int column, int row){
	int aliveNeighbors = countAliveNeighbors(column, row);
	if ((isCellAlive(column, row) && (aliveNeighbors == 2 || aliveNeighbors == 3)) ||
			(!isCellAlive(column, row) && (aliveNeighbors == 3 || aliveNeighbors == 6))){
		// life condition
		setCellAlive(column, row);
	} else {
		// death condition
		setCellDead(column, row);
	}
}

//
/**
 * Intercambia el arreglo siguente por el actual para actualizar cambios.
 */
void swapLivesArrays(){
	int* aux;
	aux = livesArrayActual;
	livesArrayActual = livesArrayNext;
	livesArrayNext = aux;

}

/**
 * Genera vidas y retorna puntero al arreglo que contiene las vidas.
 * 
 * REtorna arreglo de vidas.
 * */
int* generateInitialLives(int seed, int aliveCellsSize){
	int* cellIndexes = (int*) malloc(sizeof(int)*aliveCellsSize);

	srand(seed);
	for (int i = 0; i < aliveCellsSize; i++){
		int cellIndex = rand() % (COLUMNS*ROWS);
		bool validRandom = true;

		for (int j = 0; j < i; j++){
			if (cellIndexes[j] == cellIndex){
				// need to pick another random number
				i--;
				validRandom = false;
				break;
			}
		}
		if (validRandom)
			cellIndexes[i] = cellIndex;
	}
	return cellIndexes;

}






container_t* lives_array_to_bw_squares_vertices(){
	double start_x = -1;
	double start_y = -1;
	
	double square_ancho = 2.0/COLUMNS;
	double square_alto = 2.0/ROWS;
	int n_triangles = COLUMNS*ROWS*2;
	int array_len = n_triangles*15; // 3 vertex = 6 (x,y) points + 3 colores per vertex (9) = 15
	if (vertex_n_colors == NULL){
		float* vertex_n_colors_array = (float*) malloc(sizeof(float)*array_len);
		vertex_n_colors = (container_t*) malloc(sizeof(container_t));
		vertex_n_colors->array = vertex_n_colors_array;				
		vertex_n_colors->total_len = array_len;
		vertex_n_colors->vertex_len = n_triangles*3;
	}
	float* vertex_n_colors_array = vertex_n_colors->array;
	
	if (vertex_n_colors == NULL){
		printf("error allocating vertex\n");
		return 0;
		}
	int sqr_counter = 0;
	int trngl_counter = 0;
	int coord_counter = 0;
	
	for (start_x = -1; start_x <= 1 - square_ancho;){
		double izq = start_x;
		double der = start_x + square_ancho;
		for (start_y = -1; start_y <= 1 - square_alto;){
			double abajo = start_y;
			double arriba = start_y + square_alto;
			float color;
			if (livesArrayActual[sqr_counter] == 1){
				color = 1.0f; 
			} else {
				color = 0.0f; 
				}
			// triangulo 1
			vertex_n_colors_array[coord_counter++] = abajo;
			vertex_n_colors_array[coord_counter++] = izq;
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color			
			
			vertex_n_colors_array[coord_counter++] = abajo;
			vertex_n_colors_array[coord_counter++] = der;
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color	
			
			vertex_n_colors_array[coord_counter++] = arriba;
			vertex_n_colors_array[coord_counter++] = izq;
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color	
			
			//color


			trngl_counter++;
			// triangulo 2
			vertex_n_colors_array[coord_counter++] = abajo;
			vertex_n_colors_array[coord_counter++] = der;
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color	
			
			
			vertex_n_colors_array[coord_counter++] = arriba;
			vertex_n_colors_array[coord_counter++] = der;
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color	
			
			vertex_n_colors_array[coord_counter++] = arriba;
			vertex_n_colors_array[coord_counter++] = izq;
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color
			vertex_n_colors_array[coord_counter++] = color; //color	
			trngl_counter++;
			sqr_counter++;					
		start_y += square_ancho;
		}
	start_x +=	square_alto;
	}
	return vertex_n_colors;
}

/**
 * Inicia shaders y calcula vectores y colres iniciales.
 * 
 * */
bool init_resources(void) {
	/** SHADER COMPILATION & LINK**/
	GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
	GLuint vs, fs;
	if ((vs = create_shader("shaders/gameoflife.v.glsl", GL_VERTEX_SHADER))   == 0) return false;
	if ((fs = create_shader("shaders/gameoflife.f.glsl", GL_FRAGMENT_SHADER)) == 0) return false;
	
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		cerr << "Error in glLinkProgram" << endl;
		return false;
	}
	
	const char* attribute_name = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	attribute_name = "color";
	attribute_color = glGetAttribLocation(program, attribute_name);
	
	if (attribute_coord2d == -1) {
		cerr << "Could not bind attribute " << attribute_name << endl;
		return false;
	}

		glViewport(0, 0, win_width, win_height);

	/** VERTEX CALCULATION AND SYFF **/
	vertex_n_colors = lives_array_to_bw_squares_vertices();

	glGenBuffers(1, &vbo_triangle);
    glGenVertexArrays(1, &vao_triangle);
    glBindVertexArray(vao_triangle);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertex_n_colors->total_len, vertex_n_colors->array, GL_STATIC_DRAW);

	/* Describe our vertices array to OpenGL (it can't guess its format automatically) */
	glVertexAttribPointer(
		attribute_coord2d, // attribute
		2,                 // number of elements per vertex, here (x,y)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		5 * sizeof(float),                 // no extra data between each position
		(void*)0  // pointer to the C array
						  );
	
	glEnableVertexAttribArray(attribute_coord2d);

	glVertexAttribPointer(
		attribute_color, // attribute
		3,                 // number of elements per vertex, here (x,y)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		5 * sizeof(float),                 // no extra data between each position
		(void*)(2*sizeof(float))  // pointer to the C array
						  );
	glEnableVertexAttribArray(attribute_color);
	
	glUseProgram(program);
	return true;
}

/**
 * 
 * Calcula nuevos vertices y colores y los pasa a la gpu, luego dibuja.*/
void render(SDL_Window* window) {
	
  /* Clear the background as white */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, win_width, win_height);

	vertex_n_colors = lives_array_to_bw_squares_vertices();

	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertex_n_colors->total_len, vertex_n_colors->array, GL_STATIC_DRAW);
	float spf = 1.0/FRAMERATE;
	usleep(1000*1000*spf); // FRAMERATE
	
	/* Push each element in buffer_vertices to the vertex shader */
	glBindVertexArray(vao_triangle);
	glDrawArrays(GL_TRIANGLES, 0, vertex_n_colors->vertex_len);
	
	//glDisableVertexAttribArray(attribute_coord2d);
	//glDisableVertexAttribArray(attribute_color);

	/* Display the result */
	SDL_GL_SwapWindow(window);
}

/**
 * 
 * 
 * Loop del juego de la vida.
 * */
void mainLoop(SDL_Window* window) {
	while(1){
		// display stuff
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT)
				return;
		}
		render(window); // transfer the life information to GPU
		for (int x = 0; x < COLUMNS; x++){
			/** update life iteration (Bottle neck) **/
			for (int y = 0; y < ROWS; y++){
				checkLife(x, y); 
			}
		}
		swapLivesArrays();
	}
}

/**
 * Inicia ventana y recursos.
 * Retorna referencia a ventana.
 * 
 * */
SDL_Window* init_display_stuff(){
	/* SDL-related initialising functions */
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Game of Life: CPU",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		win_width, win_height,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_GL_CreateContext(window);

	/* Extension wrangler initialising */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << endl;
		//return EXIT_FAILURE;
	}

	/* When all init functions run without errors,
	   the program can initialise the resources */
	if (!init_resources()){
		cerr << "Error: init_resources: " << glewGetErrorString(glew_status) << endl;
		
		}
		//return EXIT_FAILURE;
	// end display
	return window;
	
	}

void init_game_data(){
		/** INICIA VARIABLES DEL JUEGO **/
	unsigned int iterCounter = 0;
	bool running = true;
	// arreglos de vidas flattened
	livesArrayActual = (int*) malloc(sizeof(int)*COLUMNS*ROWS);
	livesArrayNext = (int*) malloc(sizeof(int)*COLUMNS*ROWS);

	// se generan vidas en INITIAL_LIVES_FRACTION casillas aleatorias
	int initialAliveCellsSize = (int) COLUMNS*ROWS*INITIAL_LIVES_FRACTION;
	int* initialAliveCells = generateInitialLives(1, initialAliveCellsSize);
	for (int i = 0; i < initialAliveCellsSize; i++){
		// initial set up of lives
		livesArrayActual[initialAliveCells[i]] = 1;
	}
	free(initialAliveCells);
	}
int main() {
	/** INICIA DATOS DEL JUEGO **/
	init_game_data();
	/** INICIA DISPLAY **/
	SDL_Window* window = init_display_stuff();
	/** CORRE VIDA */
	mainLoop(window);
	
	/* If the program exits in the usual way,
   free resources and exit with a success */
	free_resources();
	return EXIT_SUCCESS;
}

void free_resources() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_triangle);
}


/** DESUSO **/
/**
 //* Dibuja el juego de la vida en ASCII.
 //*/
//void draw(int iterCounter){
	//unsigned int j = 0;
	//char c;
	//char buffer[BUFFER_SIZE];
	//memset(buffer, 0, sizeof(buffer));

///*	j += sprintf(buffer+j, " ");
	//for (int column = 0; column < COLUMNS; column++){
		//// print column numers
		//j += sprintf(buffer+j, "%d", column);
	//}
	//j += sprintf(buffer+j, "\n");*/

	//for (int row = 0; row < ROWS; row++){
		//j += sprintf(buffer+j, "%d", row); // print row numbers
		//for (int column = 0; column < COLUMNS; column++){
			//if (isCellAlive(column, row))
				//c = 254;
			//else
				//c = 'o';
			//j += sprintf(buffer+j, "%c", c);
		//}
		//j += sprintf(buffer+j, "\n");
	//}
	//printf("Round:%d\n%s\n\n", iterCounter, buffer);

	//sleep(1);
//}
