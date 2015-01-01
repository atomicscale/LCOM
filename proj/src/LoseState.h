#pragma once

#include "bitmap.h"

typedef struct {
	int done;
	Bitmap* background;
} LoseState;

LoseState* newLoseState();
void updateLoseState(LoseState* state, unsigned long scancode);
void drawLoseState(LoseState* state);
void deleteLoseState(LoseState* state);
