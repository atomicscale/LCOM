#include "video_gr.h"
#include "keyboard.h"
#include "i8042.h"
//#include "timer.h"
//#include "pixmap.h"
//#include "sprite.h"
#include "vbe.h"
#include "lmlib.h"
#include <stdint.h>
#include <machine/int86.h>

int x, y;
unsigned long p[3];
unsigned long counter;
unsigned long interrupts;

int pointer_handler() {
	unsigned long data = 0;
	short auxX, auxY;
	if (interrupts == 0) {
		while (1) {
			mouse_read(&data);
			if (data & BIT(3)) {
				p[0] = data;
				break;
			}
		}
		counter++;
		interrupts = 1;
	} else {
		mouse_read(&data);
		if (counter == 0 && data & BIT(3) == 0) {
			interrupts = 0;
			return -1;
		}
		p[counter++] = data;
		if (counter == 3) {
			interrupts = 0;
			counter = 0;
			if (Y_NEGATIVE(p[2])) {
				auxY = p[2] | 0xFF00;
			} else {
				auxY = p[2];
			}
			if (X_NEGATIVE(p[1])) {
					auxX = p[1] | 0xFF00;
				} else {
					auxX = p[1];
				}
			//
			draw_rectangle(x, y, x+10, y+10, 0);
			x += auxX;
			y -= auxY;
			draw_rectangle(x, y, x+10, y+10, 22);
		}
	}
}

int test_pointer() {
	x = 100;
	y = 100;
	vg_init(GRAPHIC_MODE);
	draw_rectangle(x, y, x+10, y+10, 22);
	int ipc_status;
	message msg;
	int receive;
	int validation = 0;
	unsigned long clean;
	unsigned long long scan_code;
	int irq_set_mouse = mouse_subscribe();
	int irq_set_kbd = kbd_subscribe();
	while (!validation) {
		receive = driver_receive(ANY, &msg, &ipc_status);
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					pointer_handler();
				}
				if (msg.NOTIFY_ARG & irq_set_kbd) {
					scan_code = kbc_read();
					if (scan_code != -1) {
						if (scan_code == KEY_UP(KEY_ESC))
							validation = 1;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	mouse_unsubscribe();
	kbd_unsubscribe();
	kbd_wait_key(KEY_ESC);
	vg_exit();
	mouse_read(&clean);
	return 0;
}
