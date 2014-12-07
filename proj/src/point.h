#pragma once

typedef struct {
	float x, y;
} Point;

Point* newPoint(float x, float y);

void deletePoint(Point * point);

void setPointLocation(Point * point, float x, float y);
