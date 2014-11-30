#ifndef __TEST5_H
#define __TEST5_H

#define H_RES             1024
#define V_RES		  	768

#include "timer.h"

typedef struct {
	int irq_set_kb;
	int irq_set_mouse;
	int irq_set_timer;
	Timer* timer;
	int validation;
	unsigned long clean;
	unsigned long scan_code;
} MouseMaze;

MouseMaze* startMouseMaze();
void updateMouseMaze(MouseMaze* maze);
void stopMouseMaze(MouseMaze* maze);

 
#endif /* __TEST5_H */
