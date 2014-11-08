#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include <minix/bitmap.h>
#include "mouse.h"
#include "i8042.h"
#include "test4.h"

int mouse_handler(packet *pack) {
	int i;
	unsigned long data = 0;
	if (pack->interrupts == 0) {
		for (i = 0; i < KBC_IO_MAX_TRIES; i++) {
			mouse_read(&data);
			pack->p[0] = data;
			if (pack->p[0] & BIT(3)) {
				break;
			}
		}
		pack->counter++;
		pack->interrupts = 1;
	} else {
		mouse_read(&data);
		if (pack->counter == 0 && data & BIT(3) == 0) {
			pack->interrupts = 0;
		}
		pack->p[pack->counter++] = data;
		if (pack->counter == 3) {
			pack->interrupts = 0;
			print(pack->p);
		}
	}
	return 0;
}

void print(unsigned long* a) {
	short p;
	printf("B1 = 0x%x , B2 = 0x%x , B3 = 0x%x \n", a[0], a[1], a[2]);
	printf("LB = %d", LEFT_B(a[0]) ? 1 : 0);
	printf("RB = %d", RIGHT_B(a[0]) ? 1 : 0);
	printf("MB = %d", MIDDLE_B(a[0]) ? 1 : 0);
	if (X_NEGATIVE(a[1])) {
		p = a[1] | 0xFF00;
	} else {
		p = a[1] & 0x00FF;
	}
	printf("X = %d", p);
	if (Y_NEGATIVE(a[2])) {
		p = a[2] | 0xFF00;
	} else {
		p = a[2] & 0x00FF;
	}
	printf("Y = %d", p);

}

int test_packet(unsigned short cnt) {
	int i = 0;
	int ipc_status;
	message msg;
	int receive;
	int irq_set = mouse_subscribe();
	packet pack;
	pack->counter = 0;
	pack->interrupts = 0;

	while (i < cnt * 3) {
		receive = driver_receive(ANY, &msg, &ipc_status);
		if (receive != 0) {
			printf("driver_receive failed with: %d", receive);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					mouse_handler(&packet);
					i++;
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	mouse_unsubscribe();
	return 0;
}

int test_async(unsigned short idle_time) {
	/* To be completed ... */
}

int test_config(void) {
	/* To be completed ... */
}

int test_gesture(short length, unsigned short tolerance) {
	/* To be completed ... */
}