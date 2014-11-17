#include "video_gr.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"
#include "i8042.h"
#include "timer.h"

void *test_init(unsigned short mode, unsigned short delay) {
	vg_init(mode);
	vg_exit();
}

int test_square(unsigned short x, unsigned short y, short size,
		unsigned long color) {

	if (size <= 0) {
		printf("Wrong argument: Size");
		return 1;
	}

	else {
		vg_init(0x105);
		draw_rectangle(x, y, x + size, y + size, color);

		int ipc_status;
		message msg;
		int validation = 0;
		int receive;

		int irq_set = kbd_subscribe();

		while (!validation) {
			receive = driver_receive(ANY, &msg, &ipc_status);
			if (receive != 0) {
				printf("driver_receive failed with: %d", receive);
				continue;
			}

			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & irq_set) {
						validation = kbd_handler_c();
					}
					break;
				default:
					break;
				}
			} else {
			}
		}
		kbd_unsubscribe();
		vg_exit();
		return 0;
	}
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, char color) {
	vg_init(0x105);

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

	int ipc_status;
	message msg;
	int validation = 0;
	int receive;

	int irq_set = kbd_subscribe();

	while (!validation) {
		receive = driver_receive(ANY, &msg, &ipc_status);
		if (receive != 0) {
			printf("driver_receive failed with: %d", receive);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					validation = kbd_handler_c();
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	kbd_unsubscribe();
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

