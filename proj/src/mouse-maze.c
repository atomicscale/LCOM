#include "video_gr.h"
#include "mouse.h"
#include "timer.h"
#include "keyboard.h"
#include "i8042.h"
#include "bitmap.h"
#include "mouse-maze.h"
#include "utilities.h"
#include <stdint.h>
#include <machine/int86.h>

const int FPS = 60;

MouseMaze* startMouseMaze() {
	MouseMaze* maze = (MouseMaze*) malloc(sizeof(MouseMaze));
	maze->irq_set_kb = kbd_subscribe();
	maze->irq_set_mouse = mouse_subscribe();
	maze->irq_set_timer = timer_subscribe_int();
	timer_set_square(0, FPS);
	//maze->test = loadBitmap("/home/lcom/lcom1415-t3g07/proj/rec/images/teste.bmp");
	maze->validation = 0;
	maze->scan_code = 0;
	maze->timer = newTimer();
	newMouse();
	return maze;
}

void stopMouseMaze(MouseMaze* maze) {
	mouse_unsubscribe();
	kbd_unsubscribe();
	timer_unsubscribe_int();
	deleteMouse();
	mouse_read(&maze->clean);
	deleteTimer(maze->timer);
	deleteBitmap(maze->test);
	free(maze);
}

void updateMouseMaze(MouseMaze* maze) {
	int ipc_status;
	message msg;

	resetTimer(maze->timer);

	int receive;
	receive = driver_receive(ANY, &msg, &ipc_status);
	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:
			if (msg.NOTIFY_ARG & maze->irq_set_timer) {
				timer_handler(maze->timer);
			}
			if (msg.NOTIFY_ARG & maze->irq_set_mouse) {
				handlerMouse();
			}
			if (msg.NOTIFY_ARG & maze->irq_set_kb) {
				maze->scan_code = kbc_read();
			}
			break;
		default:
			break;
		}
	}
	if (maze->scan_code != 0) {
		if (maze->scan_code == KEY_UP(KEY_ESC))
			maze->validation = 1;
	}
	if (maze->timer->ticked) {

		//UPDATE
		Rectangle* mouseRekt = newRectangle(
				newPoint(getMouse()->x, getMouse()->y),
				newPoint(getMouse()->x + 10, getMouse()->y + 10));
		Rectangle* trackRekt = newRectangle(newPoint(0.1 * h_res, 0.5 * v_res),
				newPoint(0.9 * h_res, 0.6 * v_res));
		Rectangle* finishRekt = newRectangle(newPoint(0.8 * h_res, 0.5 * v_res),
				newPoint(0.9 * h_res, 0.6 * v_res));

		//LOG_VAR("teste",mouseRekt->p1->x);
		if (rectanglesColliding(mouseRekt, trackRekt) || rectanglesColliding(mouseRekt, finishRekt))
			resetMouse();
		deleteRectangle(mouseRekt);
		deleteRectangle(trackRekt);
		deleteRectangle(finishRekt);
		//DRAW
		cleanScreen();
		drawMaze();
		drawMouse();
		flipScreen();
	}
}
