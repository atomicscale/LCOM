#pragma once

#include "bitmap.h"
#include "rectangle.h"

typedef enum {
	PLAY_CHOSEN, EXIT_CHOSEN
} MainMenuAction;

typedef struct {
	int done;
	Bitmap* background;
	int mouseOnPlay;
	Rectangle* playButton;
	int mouseOnExit;
	Rectangle* exitButton;

	int action;
} MainMenuState;

MainMenuState* newMainMenuState();
int updateMainMenuState(MainMenuState* state, unsigned long scancode);
void drawMainMenuState(MainMenuState* state);
void deleteMainMenuState(MainMenuState* state);
