#include "i8042.h"
#include "mouse.h"
#include "video_gr.h"

static int hook_id = MOUSE_HOOK_ID;

Mouse* mouse = NULL;

void newMouse();

void newMouse() {
	mouse = (Mouse*) malloc(sizeof(Mouse));
	mouse->x = 100;
	mouse->y = 100;
	mouse->xSign = 0;
	mouse->ySign = 0;
	mouse->deltaX = 0;
	mouse->deltaY = 0;
	mouse->counter = 0;
	mouse->interrupts = 0;
	mouse->leftButtonDown = 0;
	mouse->middleButtonDown = 0;
	mouse->rightButtonDown = 0;
	mouse->color = 22;
}

Mouse* getMouse() {
	return mouse;
}

void updateMouse() {
	/*mouse->xSign = X_NEGATIVE(mouse->packet[1]);
	mouse->ySign = Y_NEGATIVE(mouse->packet[2]);
	if (mouse->ySign)
		mouse->deltaY = mouse->packet[2] | 0xFF00;
	else
		mouse->deltaY = mouse->packet[2];
	if (mouse->xSign)
		mouse->deltaX = mouse->packet[1] | 0xFF00;
	else
		mouse->deltaX = mouse->packet[1];*/
	mouse->deltaX = X_VAL(mouse->packet);
	mouse->deltaY = Y_VAL(mouse->packet);
	mouse->x += mouse->deltaX;
	mouse->y -= mouse->deltaY;
	mouse->leftButtonDown = LEFT_B(mouse->packet[0]);
	mouse->middleButtonDown = MIDDLE_B(mouse->packet[0]);
	mouse->rightButtonDown = RIGHT_B(mouse->packet[0]);
}

void drawMouse() {
	//memset(getGraphicsBuffer(), 0, 1024*768);
	draw_rectangle(mouse->x, mouse->y, mouse->x + 10, mouse->y + 10,
			mouse->color);
}

void deleteMouse() {
	free(mouse);
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
}

void handlerMouse(){
	int i;
	unsigned long data = 0;
	if (mouse->interrupts == 0) {
		for (i = 0; i < KBC_IO_MAX_TRIES; i++) {
			mouse_read(&data);
			if (data & BIT(3)) {
				mouse->packet[0] = data;
				break;
			}
		}
		mouse->counter++;
		mouse->interrupts = 1;
	} else {
		mouse_read(&data);
		if (mouse->counter == 0 && data & BIT(3) == 0) {
			mouse->interrupts = 0;
			return;
		}
		mouse->packet[mouse->counter++] = data;
		if (mouse->counter == 3) {
			mouse->counter = 0;
			updateMouse();
			//print(mouse->packet);
		}
	}
	return;
}

int mouse_write(unsigned char cmd) {
	unsigned long stat;
	int i = 0;

	for (i = 0; i < KBC_IO_MAX_TRIES; i++) {
		sys_outb(CMD_REG, WRITE_BYTE);
		tickdelay(micros_to_ticks(DELAY_US));
		sys_outb(IN_BUF, cmd); /* no args command */
		tickdelay(micros_to_ticks(DELAY_US));
		sys_inb(OUT_BUF, &stat); /* assuming it returns OK */

		if (stat == ACK) {
			return 0;
		}
	}
}

int mouse_read(unsigned long* val) {
	unsigned long read = 0;
	int i = 0;
	for (i = 0; i < KBC_IO_MAX_TRIES; i++) {
		sys_inb(STAT_REG, &read);
		if ((read & OBF) && (read & AUX)) {
			if (sys_inb(OUT_BUF, val) != 0) {
				return -1;
			}
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return -1;
}

int mouse_subscribe() {
	if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)
			!= 0) {
		printf("\nMouse_subscribe() failed \n");
		return -1;
	}
	if (sys_irqenable(&hook_id) != 0) {
		printf("\nMouse_subscribe() failed \n");
		return -1;
	}
	mouse_write(STREAM_MODE);
	mouse_write(ENABLE_PACKETS);
	return BIT(MOUSE_HOOK_ID);
}

int mouse_unsubscribe() {
	if (sys_irqdisable(&hook_id) != 0) {
		printf("\nMouse_unsubscribe failed \n");
		return 1;
	}
	if (sys_irqrmpolicy(&hook_id) != 0) {
		printf("\nMouse_unsubscribe failed \n");
		return 1;
	}
	return 0;
}

