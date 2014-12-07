#include "point.h"

Point* newPoint(float x, float y){
	Point * point = (Point*) malloc(sizeof(Point));
	point->x = x;
	point->y = y;
	return point;
}

void deletePoint(Point * point){
	free(point);
}

void setPointLocation(Point* point, float x, float y) {
	point->x = x;
	point->y = y;
}
