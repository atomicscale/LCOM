#pragma once

#include "bitmap.h"
#include "labyrinth.h"

typedef struct {
	int done;
	int win;
	int lose;
	int counter;
	int timer;
	int nigth;
	int mouse;
	Labyrinth* lab;
	Bitmap* background;
	Rectangle* rect;
} GameState;

GameState* newGameState(int mouse, int nigth);
void updateGameState(GameState* state, unsigned long scancode);
void drawGameState(GameState* state);
void deleteGameState(GameState* state);
