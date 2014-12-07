#include "rectangle.h"

Rectangle* newRectangle(Point * p1, Point * p2) {
	Rectangle * rectangle = (Rectangle*) malloc(sizeof(Rectangle));
	rectangle->p1 = p1;
	rectangle->p2 = p2;
	return rectangle;
}

void deleteRectangle(Rectangle * rectangle) {
	free(rectangle);
}

void setRecLocation(Rectangle * rectangle, Point * dest) {
	float width = rectangle->p2->x - rectangle->p1->x;
	float height = rectangle->p2->y - rectangle->p1->y;

	rectangle->p1 = dest;
	rectangle->p2->x = dest->x + width;
	rectangle->p2->y = dest->y + height;
}

void moveRectangle(Rectangle* rectangle, Point * delta) {
	float width = rectangle->p2->x - rectangle->p1->x;
	float height = rectangle->p2->y - rectangle->p1->y;

	rectangle->p1->x += delta->x;
	rectangle->p1->y += delta->y;
	rectangle->p2->x += delta->x;
	rectangle->p2->y += delta->y;
}

int rectanglesColliding(Rectangle* r1, Rectangle* r2) {
	return !(r1->p2->y < r2->p1->y || r1->p1->y > r2->p2->y
			|| r1->p2->x < r2->p1->x || r1->p1->x > r2->p2->x);
}
