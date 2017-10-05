/*
 * CPU.h
 *
 *  Created on: 24-09-2017
 *      Author: vichoko
 */

#ifndef CPU_H_
#define CPU_H_

class CPU {
public:
	CPU();
	virtual ~CPU();
};

#endif /* CPU_H_ */

int isCellAlive(int column, int row);

// graphics
/// display
typedef struct {
	float* array;
	int total_len;
	int vertex_len;
	} container_t;



void free_resources();


