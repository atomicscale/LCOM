#pragma once

#include "point.h"

typedef struct {
	int x;
	int y;
	int w;
	int h;
} Rectangle;

Rectangle* newRectangle(int x, int y, int w, int h);

void deleteRectangle(Rectangle * rectangle);

void setRecLocation(Rectangle * rectangle, int x, int y);

void moveRectangle(Rectangle* rectangle, int x, int y);

void drawRectangle(Rectangle* rectangle, int color, char* buf);

int rectanglesColliding(Rectangle* r1, Rectangle* r2);

int rectangleInside(Rectangle* r1, Rectangle* r2);
