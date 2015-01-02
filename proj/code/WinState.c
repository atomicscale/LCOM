#include "WinState.h"
#include "keyboard.h"

WinState* newWinState() {
	WinState* state = (WinState*) malloc(sizeof(WinState));

	state->done = 0;
	state->background = loadBitmap(
			"/home/lcom/lcom1415-t3g07/proj/rec/images/mouse-win.bmp");
	return state;
}

void updateWinState(WinState* state, unsigned long scancode) {
	if (scancode == KEY_DOWN(KEY_ENTER))
		state->done = 1;
}

void drawWinState(WinState* state) {
	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
}

void deleteWinState(WinState* state) {
	deleteBitmap(state->background);
	free(state);
}
