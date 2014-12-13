#include "i8042.h"
#include "mouse.h"
#include "video_gr.h"
#include "rectangle.h"

static int hook_id = MOUSE_HOOK_ID;

Mouse* mouse = NULL;

void newMouse();

void newMouse() {
	mouse = (Mouse*) malloc(sizeof(Mouse));
	mouse->x = 0.15 * getH_res();
	mouse->y = 0.55 * getV_res();
	mouse->xSign = 0;
	mouse->ySign = 0;
	mouse->deltaX = 0;
	mouse->deltaY = 0;
	mouse->counter = 0;
	mouse->interrupts = 0;
	mouse->leftButtonDown = 0;
	mouse->middleButtonDown = 0;
	mouse->rightButtonDown = 0;
	mouse->color = rgb(0, 0, 255);
}

Mouse* getMouse() {
	if (!mouse)
		newMouse();
	return mouse;
}

void setMouseColor(int color){
	mouse->color = color;
}

void updateMouse() {
	mouse->xSign = X_NEGATIVE(mouse->packet[1]);
	mouse->ySign = Y_NEGATIVE(mouse->packet[2]);
	if (mouse->ySign)
		mouse->deltaY = mouse->packet[2] | 0xFF00;
	else
		mouse->deltaY = mouse->packet[2];
	if (mouse->xSign)
		mouse->deltaX = mouse->packet[1] | 0xFF00;
	else
		mouse->deltaX = mouse->packet[1];
	mouse->x += mouse->deltaX;
	mouse->y -= mouse->deltaY;
	mouse->leftButtonDown = LEFT_B(mouse->packet[0]);
	mouse->middleButtonDown = MIDDLE_B(mouse->packet[0]);
	mouse->rightButtonDown = RIGHT_B(mouse->packet[0]);
}

void drawMouse() {
	draw_rectangle(mouse->x, mouse->y, mouse->x + 10, mouse->y + 10,
			mouse->color, getGraphicsBuffer());
}

void deleteMouse() {
	free(mouse);
}

void handlerMouse() {
	unsigned long data;
	mouse_read(&data);
	mouse->packet[mouse->counter] = data;

	if (mouse->counter == 0 && !(BIT(3) & mouse->packet[0]))
		return;
	mouse->counter = (mouse->counter + 1) % 3;
	mouse->interrupts++;
	if (mouse->counter != 0)
		return;
	updateMouse();
}

int mouse_write(unsigned char cmd) {
	unsigned long stat;
	int i = 0;

	for (i = 0; i < KBC_IO_MAX_TRIES; i++) {
		sys_outb(CMD_REG, WRITE_BYTE);
		tickdelay(micros_to_ticks(DELAY_US));
		sys_outb(IN_BUF, cmd); /* no args command */
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

void resetMouse(){
	mouse->x = 0.15 * getH_res();
	mouse->y = 0.55 * getV_res();
}

int mouse_outH(int aux)
{
	if (aux > getH_res())
		return getH_res();
	if (aux < getH_res())
		return 0;
	return aux;
}

int mouse_outV(int aux)
{
	if (aux > getV_res())
		return getV_res();
	if (aux < getV_res())
		return 0;
	return aux;
}
