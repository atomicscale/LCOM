#pragma once

#include "rectangle.h"

typedef struct {
	Rectangle* rec;
	Rectangle* finish;
} Labyrinth;

Labyrinth* newLabyrinth();

void drawLabyrinth(Labyrinth* lab);

void deleteLabyrinth(Labyrinth* lab);
