#pragma once

#include "bitmap.h"
#include "labyrinth.h"

typedef struct {
	int done;
	Labyrinth* lab;
	Bitmap* background;
} GameState;

GameState* newGameState();
void updateGameState(GameState* state, unsigned long scancode);
void drawGameState(GameState* state);
void deleteGameState(GameState* state);
