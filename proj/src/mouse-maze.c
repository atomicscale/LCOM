#include "video_gr.h"
#include "mouse.h"
#include "timer.h"
#include "keyboard.h"
#include "i8042.h"
#include "bitmap.h"
#include "mouse-maze.h"
#include "utilities.h"
#include "MainMenuState.h"
#include <stdint.h>
#include <machine/int86.h>

#include "MainMenuState.h"
#include "GameState.h"

const int FPS = 60;

void checkIfStateIsDone(MouseMaze* game);
void deleteCurrentState(MouseMaze* game);

MouseMaze* startMouseMaze() {
	MouseMaze* maze = (MouseMaze*) malloc(sizeof(MouseMaze));
	maze->irq_set_kb = kbd_subscribe();
	maze->irq_set_mouse = mouse_subscribe();
	maze->irq_set_timer = timer_subscribe_int();
	timer_set_square(0, FPS);
	maze->validation = 0;
	maze->scan_code = 0;
	maze->draw = 1;
	maze->timer = newTimer();
	newMouse();
	maze->currentState = MAIN_MENU_STATE;
	maze->state = newMainMenuState();
	return maze;
}

void stopMouseMaze(MouseMaze* maze) {
	deleteCurrentState(maze);
	mouse_unsubscribe();
	kbd_unsubscribe();
	timer_unsubscribe_int();
	deleteMouse();
	mouse_read(&maze->clean);
	deleteTimer(maze->timer);
	free(maze);
}

void drawMouseMaze(MouseMaze* maze) {
	cleanScreen();
	switch (maze->currentState) {
	case MAIN_MENU_STATE:
		drawMainMenuState(maze->state);
		break;
	case GAME_STATE:
		drawGameState(maze->state);
		break;
	default:
		break;
	}
}

void updateMouseMaze(MouseMaze* maze) {
	int ipc_status;
	message msg;

	resetTimer(maze->timer);

	int receive;
	receive = driver_receive(ANY, &msg, &ipc_status);
	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:
			if (msg.NOTIFY_ARG & maze->irq_set_timer) {
				timer_handler(maze->timer);
			}
			if (msg.NOTIFY_ARG & maze->irq_set_mouse) {
				handlerMouse();
			}
			if (msg.NOTIFY_ARG & maze->irq_set_kb) {
				maze->scan_code = kbc_read();
			}
			break;
		default:
			break;
		}
	}
	if (maze->timer->ticked) {
		switch (maze->currentState) {
		case MAIN_MENU_STATE:
			updateMainMenuState(maze->state, maze->scan_code);
			break;
		case GAME_STATE:
			updateGameState(maze->state, maze->scan_code);
			break;
		default:
			break;
		}
		maze->scan_code = 0;
		maze->draw = 1;
		drawMouseMaze(maze);
		drawMouse();
		flipScreen();
	}
	checkIfStateIsDone(maze);
}

void changeState(MouseMaze* maze, State newState) {
	// deleting current state
	deleteCurrentState(maze);

	// changing current state
	maze->currentState = newState;

	// creating new state
	switch (maze->currentState) {
	case MAIN_MENU_STATE:
		maze->state = newMainMenuState();
		break;
	case GAME_STATE:
		maze->state = newGameState();
		break;
	default:
		break;
	}
	maze->draw = 1;
}

void checkIfStateIsDone(MouseMaze* maze) {
	switch (maze->currentState) {
	case MAIN_MENU_STATE:
		if (((MainMenuState*) (maze->state))->done) {
			int action = ((MainMenuState*) (maze->state))->action;

			switch (action) {
			case PLAY_CHOSEN:
				changeState(maze, GAME_STATE);
				break;
			case EXIT_CHOSEN:
				maze->validation = 1;
				break;
			}
		}
		break;
	case GAME_STATE:
		if (((GameState*) (maze->state))->done)
			changeState(maze, MAIN_MENU_STATE);
	default:
		break;
	}
}

void deleteCurrentState(MouseMaze* maze) {
	switch (maze->currentState) {
	case MAIN_MENU_STATE:
		deleteMainMenuState(maze->state);
		break;
	case GAME_STATE:
		deleteGameState(maze->state);
		break;
	default:
		break;
	}
}
