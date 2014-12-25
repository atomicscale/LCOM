#include "MainMenuState.h"

#include "video_gr.h"
#include "keyboard.h"
#include "mouse.h"
#include "utilities.h"

MainMenuState* newMainMenuState() {
	MainMenuState* state = (MainMenuState*) malloc(sizeof(MainMenuState));

	state->done = 0;
	state->action=0;
	state->background = loadBitmap(
			"/home/lcom/lcom1415-t3g07/proj/rec/images/main-menu1.bmp");

	// these numbers are just meant to create the buttons boundaries
	double w = .075, hi = .38, hf = hi + .10;
	int x1 = getH_res() / 2 - getH_res() * w;
	int x2 = getH_res() / 2 + getH_res() * w;
	int y1 = getV_res() * hi;
	int y2 = getV_res() * hf;
	state->playButton = newRectangle(x1, y1, x2 - x1, y2 - y1);
	state->mouseOnPlay = 0;

	hi = .59, hf = hi + .10;
	y1 = getV_res() * hi;
	y2 = getV_res() * hf;
	state->exitButton = newRectangle(x1, y1, x2 - x1, y2 - y1);
	state->mouseOnExit = 0;

	return state;
}

int updateMainMenuState(MainMenuState* state, unsigned long scancode) {
	int draw = 0;

	// if ESC has been pressed, quit
	if (scancode == KEY_DOWN(KEY_ESC)) {
		state->action = 1;
		state->done = 1;
	}

	// if mouse is inside the play button rectangle (boundaries)
	if (rectangleInside(getMouse()->rect, state->playButton)) {
		state->mouseOnPlay = 1;
		if (getMouse()->leftButtonDown) {
			state->action = 0;
			state->done = 1;
		}
	} else
		state->mouseOnPlay = 0;

	// if mouse is inside the exit button rectangle (boundaries)
	if (rectangleInside(getMouse()->rect, state->exitButton)) {
		state->mouseOnExit = 1;

		// and left mouse button has been released
		if (getMouse()->leftButtonDown) {
			state->action = 1;
			state->done = 1;
		}
	} else
		state->mouseOnExit = 0;

	return draw;
}

void drawMainMenuState(MainMenuState* state) {
	drawBitmap(state->background, 0, 0, ALIGN_LEFT);

	if (state->mouseOnPlay)
		drawRectangle(state->playButton, rgb(255, 0, 0), getGraphicsBuffer());
	else if (state->mouseOnExit)
		drawRectangle(state->exitButton, rgb(255, 0, 0), getGraphicsBuffer());
}

void deleteMainMenuState(MainMenuState* state) {
	deleteBitmap(state->background);
	deleteRectangle(state->playButton);
	deleteRectangle(state->exitButton);

	free(state);
}
