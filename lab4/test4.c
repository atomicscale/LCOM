#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include <minix/bitmap.h>
#include "mouse.h"
#include "i8042.h"
#include "test4.h"

unsigned long p[3];
unsigned long counter;
unsigned long interrupts;

short sumOfX;
short Y;

int gesture_handler(short length, unsigned short tolerance) {
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
			//prints
			printf("sumX: %d left: %d ", sumOfX, LEFT_B(p[0]));
			if (Y_NEGATIVE(p[2])) {
				auxY = p[2] | 0xFF00;
			} else {
				auxY = p[2];
			}
			printf("Y=%d ", auxY);
			if (X_NEGATIVE(p[1])) {
					auxX = p[1] | 0xFF00;
				} else {
					auxX = p[1];
				}
				printf("X=%d \n \n", auxX);
			//
			Y += abs(auxY);
			if (Y < tolerance && LEFT_B(p[0])) {
				sumOfX += auxX;
				if (length > 0){
					if (auxX < 0)
						sumOfX = 0;
					if (sumOfX >= length)
						return 0;
				}
				else
				{
					if (auxX > 0)
						sumOfX = 0;
					if (sumOfX < length)
						return 0;
				}

			}
			if (Y > tolerance || !LEFT_B(p[0])) {
				sumOfX = 0;
				Y = 0;
				return -1;
			}
		}
		return -1;
	}
}

int mouse_handler() {
	int i;
	unsigned long data = 0;
	if (interrupts == 0) {
		for (i = 0; i < KBC_IO_MAX_TRIES; i++) {
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
			return 0;
		}
		p[counter++] = data;
		if (counter == 3) {
			interrupts = 0;
			print(p);
		}
	}
	return 0;
}

void print(unsigned long* a) {
	short p;
	printf("\tB1=0x%x B2=0x%x B3=0x%x ", a[0], a[1], a[2]);
	printf("LB=%d ", LEFT_B(a[0]) ? 1 : 0);
	printf("RB=%d ", RIGHT_B(a[0]) ? 1 : 0);
	printf("MB=%d ", MIDDLE_B(a[0]) ? 1 : 0);
	printf("XOV=%d ", X_OVERFLOW(a[0]) ? 1 : 0);
	printf("YOV=%d ", Y_OVERFLOW(a[0]) ? 1 : 0);
	if (X_NEGATIVE(a[1])) {
		p = a[1] | 0xFF00;
	} else {
		p = a[1];
	}
	printf("X=%d ", p);
	if (Y_NEGATIVE(a[2])) {
		p = a[2] | 0xFF00;
	} else {
		p = a[2];
	}
	printf("Y=%d \n", p);
	counter = 0;
}

int test_packet(unsigned short cnt) {
	int i = 0;
	int ipc_status;
	message msg;
	int receive;
	int irq_set = mouse_subscribe();
	unsigned long clean;
	counter = 0;
	interrupts = 0;
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
					mouse_handler();
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
	printf("\n\tpress ANY KEY to continue\n");
	mouse_read(&clean); //Clean the buffer
	return 0;
}

int test_async(unsigned short idle_time) {
	int ipc_status;
	message msg;
	int time = 0;
	int receive;
	unsigned long clean;
	int irq_set_mouse = mouse_subscribe();
	int irq_set_timer = timer_subscribe_int();
	counter = 0;
	interrupts = 0;
	while (time < idle_time * 60) {
		receive = driver_receive(ANY, &msg, &ipc_status);
		if (receive != 0) {
			printf("driver_receive failed with: %d", receive);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					mouse_handler();
					time = 0;
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					time++;
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	mouse_unsubscribe();
	timer_unsubscribe_int();
	printf("\n\tpress ANY KEY to continue\n");
	mouse_read(&clean);
	return 0;
}

int test_config(void) {
	printf("\n\tTouch in the mouse\n\n");
	unsigned long data, res;
	unsigned char a[3];
	mouse_subscribe();
	mouse_write(MOUSE_STATUS);
	mouse_read(&data);
	while (1) {
		mouse_read(&data);
		if ((BIT(7) & data) && (BIT(3) & res))
			break;
	}
	a[0] = data;
	while (1) {
		mouse_read(&data);
		if (data <= 3)
			break;
	}
	a[1] = data;
	tickdelay(micros_to_ticks(DELAY_US));
	if (mouse_read(&data) != 0)
		return -1;
	a[2] = data;
	printf("\n\tCONFIGURATION\n");
	printf(
			"\tMode: %s\n\tEnable: %d\n\tScaling: %s\n\tLB: %d\n\tMB: %d\n\tRB: %d\n\tResolution: %d count/mm\n\tSample Rate: %d\n",
			MODE(a[0]) ? "Remote" : "Stream", ENABLE(a[0]),
			SCALING(a[0]) ? "1:1" : "2:1", LEFT(a[0]), MIDDLE(a[0]),
			RIGHT(a[0]), RESOLUTION(a[1]), RATE(a[2]));
	mouse_unsubscribe();
	printf("\n\tpress ANY KEY to continue\n");
	mouse_read(&res); /* clear out buffer */
}

int test_gesture(short length, unsigned short tolerance) {
	int ipc_status;
	message msg;
	int receive;
	int validation = 1;
	sumOfX = 0;
	int irq_set = mouse_subscribe();
	unsigned long clean;
	counter = 0;
	interrupts = 0;
	while (validation) {
		receive = driver_receive(ANY, &msg, &ipc_status);
		if (receive != 0) {
			printf("driver_receive failed with: %d", receive);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					validation = gesture_handler(length, tolerance);
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	mouse_unsubscribe();
	printf("\n\tpress ANY KEY to continue\n");
	mouse_read(&clean); //Clean the buffer
	return 0;
}
