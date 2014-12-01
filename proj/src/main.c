#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "mouse-maze.h"
#include "video_gr.h"
#include <assert.h>
#include <stdio.h>
#include "utilities.h"

FILE* logfile = NULL;

void initLog(){
	logfile = fopen("home/lcom/log.txt", "w");
	assert(logfile != NULL);

	LOG("test", "mensagem teste");
}

int main(int argc, char** argv) {
	initLog();//
	sef_startup();
	vg_init(GRAPHIC_MODE);
	MouseMaze* maze = (MouseMaze*)startMouseMaze();
	while(!maze->validation){
		updateMouseMaze(maze);
	}
	stopMouseMaze(maze);
	vg_exit();
	fclose(logfile);//
	return 0;
}
