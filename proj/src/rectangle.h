#pragma once

#include "point.h"

typedef struct {
	Point * p1;
	Point * p2;
} Rectangle;

Rectangle* newRectangle(Point * p1, Point * p2);

void deleteRectangle(Rectangle * rectangle);

void setRecLocation(Rectangle * rectangle, Point * dest);

void moveRectangle (Rectangle* rectangle, Point * delta);

int rectanglesColliding(Rectangle* r1, Rectangle* r2);
