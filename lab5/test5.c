#include "video_gr.h"
#include "keyboard.h"
#include "i8042.h"
#include "timer.h"

void *test_init(unsigned short mode, unsigned short delay) {
	vg_init(mode);
	wait_seconds(delay);
	if(vg_exit())
		printf("Virtual address VRAM was mapped to: %u", *video_mem);
	else
		printf("Virtual address VRAM was mapped to: NULL");
}

int test_square(unsigned short x, unsigned short y, short size,
		unsigned long color) {

	if (size <= 0) {
		printf("Wrong argument: Size");
		return 1;
	}

	else {
		vg_init(GRAPHIC_MODE);
		draw_rectangle(x, y, x + size, y + size, color);
		kbd_wait_key(KEY_ESC);
		vg_exit();
		return 0;
	}
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, char color) {
	vg_init(GRAPHIC_MODE);

	int dx = abs(xf - xi), sx = xi < xf ? 1 : -1;
	int dy = abs(yf - yi), sy = yi < yf ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	for (;;) {
		draw_pixel(xi, yi, color);
		if (xi == xf && yi == yf)
			break;
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			xi += sx;
		}
		if (e2 < dy) {
			err += dx;
			yi += sy;
		}
	}
	kbd_wait_key(KEY_ESC);
	vg_exit();
	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	/* To be completed */

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {

	/* To be completed */

}

