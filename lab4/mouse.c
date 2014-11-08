#include "kbc.h"
#include "i8042.h"
#include "mouse.h"

static int hook_id;

int mouse_write(unsigned char cmd) {
	if (kbc_write2(CMD_REG, WRITE_BYTE) == 0 && kbc_write2(DATA, cmd) == 0)
		return 0;
	else {
		printf("mouse_write: write_kbc failed\n");
		return -1;
	}
}

int mouse_read(unsigned long* val) {
	unsigned long read = 0;
	int i = 0;
	for (i = 0; i < KBC_IO_MAX_TRIES; i++) {
		sys_inb(STAT_REG, &read);
		if (read & OBF && read & AUX) {
			if (sys_inb(OUT_BUF, val) != 0) {
				return -1;
			}
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return 0;
}

int mouse_subscribe() {
	mouse_write(DISABLE_STREAM);
	if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0){
			printf("\nMouse_subscribe() failed \n");
			return -1;
	}
	if (sys_irqenable(&hook_id) != 0) {
			printf("\nMouse_subscribe() failed \n");
			return -1;
	}
	mouse_write(STREAM_MODE);
	mouse_write(ENABLE_PACKETS);
	return BIT(5);
}

int mouse_unsubscribe() {
	if (sys_irqdisable(&hook_id) != 0 && sys_irqrmpolicy(&hook_id)) {
		printf("\nMouse_unsubscribe failed \n");
		return 1;
	}
	return 0;
}




