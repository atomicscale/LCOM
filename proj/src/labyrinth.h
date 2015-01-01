#pragma once

#include "rectangle.h"

typedef struct {
	Rectangle* rec1;
	Rectangle* rec2;
	Rectangle* rec3;
	Rectangle* rec4;
	Rectangle* rec5;
	Rectangle* rec6;
	Rectangle* finish;
} Labyrinth;

Labyrinth* newLabyrinth();

void drawLabyrinth(Labyrinth* lab);

int insideLab(Labyrinth* lab);

void deleteLabyrinth(Labyrinth* lab);
