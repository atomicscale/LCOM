#pragma once

#include "bitmap.h"

typedef struct {
	int done;
	Bitmap* background;
} WinState;

WinState* newWinState();
void updateWinState(WinState* state, unsigned long scancode);
void drawWinState(WinState* state);
void deleteWinState(WinState* state);
