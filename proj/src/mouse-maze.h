#ifndef __TEST5_H
#define __TEST5_H


#include "timer.h"

typedef enum{
	MAIN_MENU_STATE, GAME_STATE
} State;

typedef struct {
	int irq_set_kb;
	int irq_set_mouse;
	int irq_set_timer;
	Timer* timer;
	int validation;
	int draw;
	unsigned long clean;
	unsigned long scan_code;
	State currentState;
	void* state;
} MouseMaze;

MouseMaze* startMouseMaze();
void updateMouseMaze(MouseMaze* maze);
void stopMouseMaze(MouseMaze* maze);

 
#endif /* __TEST5_H */
