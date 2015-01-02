#include "LoseState.h"
#include "keyboard.h"

LoseState* newLoseState() {
	LoseState* state = (LoseState*) malloc(sizeof(LoseState));

	state->done = 0;
	state->background = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/mouse-lose.bmp");
	return state;
}

void updateLoseState(LoseState* state, unsigned long scancode) {
	if (scancode == KEY_DOWN(KEY_ENTER))
		state->done = 1;
}

void drawLoseState(LoseState* state) {
	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
}

void deleteLoseState(LoseState* state) {
	deleteBitmap(state->background);
	free(state);
}
