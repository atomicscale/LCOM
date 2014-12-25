#include "GameState.h"
#include "keyboard.h"
#include "mouse.h"

GameState* newGameState() {
	GameState* state = (GameState*) malloc(sizeof(GameState));

	state->done = 0;
	state->background = loadBitmap(
			"/home/lcom/lcom1415-t3g07/proj/rec/images/game-background.bmp");
	state->lab = newLabyrinth();
	return state;
}

void updateGameState(GameState* state, unsigned long scancode) {
	if (scancode == KEY_DOWN(KEY_ESC))
		state->done = 1;
	if (!rectangleInside(getMouse()->rect, state->lab->rec))
		resetMouse();
	if (rectanglesColliding(getMouse()->rect, state->lab->finish))
		state->done = 1;
}

void drawGameState(GameState* state) {
	drawBitmap(state->background, 0, 0, ALIGN_LEFT);
	drawLabyrinth(state->lab);
}

void deleteGameState(GameState* state) {
	deleteBitmap(state->background);
	free(state);
}
