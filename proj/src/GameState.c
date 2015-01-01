#include "GameState.h"
#include "keyboard.h"
#include "mouse.h"
#include "rtc.h"
#include "utilities.h"

GameState* newGameState(int mouse, int nigth) {
	GameState* state = (GameState*) malloc(sizeof(GameState));
	rtc_write_alarm(20);
	rtc_enable();
	state->timer = 20;
	state->done = 0;
	state->win = 0;
	state->lose = 0;
	state->nigth = nigth;
	state->mouse = mouse;
	state->counter = 0;
	state->background = loadBitmap(
			"/home/lcom/lcom1415-t3g07/proj/rec/images/game-background.bmp");
	state->lab = newLabyrinth();
	resetMouse();
	return state;
}

void updateGameState(GameState* state, unsigned long scancode) {
	if (state->counter <= 15) {
		state->counter++;
	}
	if (state->mouse) {
		if (scancode == KEY_DOWN(KEY_D))
			getMouse()->x += 10;
		else if (scancode == KEY_DOWN(KEY_A))
			getMouse()->x -= 10;
		else if (scancode == KEY_DOWN(KEY_W))
			getMouse()->y -= 10;
		else if (scancode == KEY_DOWN(KEY_S))
			getMouse()->y += 10;
	}
	if (scancode == KEY_DOWN(KEY_ESC))
		state->done = 1;
	else if (scancode == KEY_DOWN(KEY_P))
		if (state->nigth)
			state->nigth = 0;
		else
			state->nigth = 1;
	else if (scancode == KEY_DOWN(KEY_M))
		if (state->mouse)
			state->mouse = 0;
		else
			state->mouse = 1;
	setRecLocation(getMouse()->rect, getMouse()->x, getMouse()->y);
	if (insideLab(state->lab)) {
		resetMouse();
		if (state->counter >= 15) {
			state->lose = 1;
			state->done = 1;
		}
	}
	if (rectanglesColliding(getMouse()->rect, state->lab->finish)) {
		state->win = 1;
		state->done = 1;
	}
}

void drawGameState(GameState* state) {
	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
	drawLabyrinth(state->lab);
	if (state->nigth)
		see_circle(getMouse()->x, getMouse()->y, 50, rgb(0, 0, 0),
				getGraphicsBuffer());
	draw_rectangle(0, 10, state->timer*40, 40, rgb(255, 255, 0), getGraphicsBuffer());
}

void deleteGameState(GameState* state) {
	rtc_disable();
	deleteBitmap(state->background);
	free(state);
}
