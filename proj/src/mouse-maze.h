#ifndef __TEST5_H
#define __TEST5_H


#include "timer.h"
#include "rtc.h"

typedef enum{
	MAIN_MENU_STATE, GAME_STATE, MODE_STATE, WIN_STATE, LOSE_STATE
} State;

typedef struct {
	int irq_set_kb;
	int irq_set_mouse;
	int irq_set_timer;
	int irq_set_rtc;
	Timer* timer;
	rtc_time_t rtc;
	int validation;
	int draw;
	unsigned long clean;
	unsigned long scan_code;
	State currentState;
	void* state;
} MouseMaze;

void handler_rtc(MouseMaze* maze);

MouseMaze* startMouseMaze();
void updateMouseMaze(MouseMaze* maze);
void stopMouseMaze(MouseMaze* maze);

void changeState(MouseMaze* maze, State newState);
void checkIfStateIsDone(MouseMaze* maze);
void deleteCurrentState(MouseMaze* game);


#endif /* __TEST5_H */
