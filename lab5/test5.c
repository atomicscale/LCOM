#include "video_gr.h"
#include "keyboard.h"
#include "i8042.h"
#include "timer.h"
#include "read_xpm.c"
#include "pixmap.h"

void *test_init(unsigned short mode, unsigned short delay) {
	vg_init(mode);
	wait_seconds(delay);
	if (vg_exit())
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
	// algorithm from http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
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
	//
	kbd_wait_key(KEY_ESC);
	vg_exit();
	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	if (xi > H_RES || yi > V_RES) {
		printf("ERROR! xi > H_RES or yi > V_RES\n");
		return 1;
	}
	vg_init(GRAPHIC_MODE);
	int x = 1, y = 1;
	int width, height;
	char *map;
	map = read_xpm(xpm, &width, &height);
	// get the pix map from the XPM
	while (x <= width && y <= height) {
		draw_pixel(x+xi, y+yi, *map);
		if (x == width){
			x = 0;
			y++;
		}
		x++;
		map++;
	}

	kbd_wait_key(KEY_ESC);
	vg_exit();
	return 0;

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {

	/* To be completed */

}

