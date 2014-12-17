#include "rectangle.h"
#include "labyrinth.h"
#include "video_gr.h"

Labyrinth* newLabyrinth(){
	Labyrinth* lab = (Labyrinth*) malloc(sizeof(Labyrinth));
	lab->rec = newRectangle(0.1 * getH_res(), 0.5 * getV_res(), 0.8 * getH_res(), 0.1 * getV_res());
	lab->finish = newRectangle(0.8 * getH_res(), 0.5 * getV_res(), 0.1 * getH_res(), 0.1 * getV_res());
	return lab;
}

void drawLabyrinth(Labyrinth* lab) {
	drawRectangle(lab->rec, rgb(0, 255, 200), getGraphicsBuffer());
	drawRectangle(lab->finish, rgb(255, 0, 0), getGraphicsBuffer());
}

void deleteLabyrinth(Labyrinth* lab) {
	free(lab);
}
