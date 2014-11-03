#include "kbc.h"
#include "i8042.h"
#include "mouse.h"

static int hook_id;

int mouse_write(unsigned char cmd) {
	if (kbc_write(CMD_REG, WRITE_BYTE) == 0 && kbc_write(DATA, cmd) == 0)
		return 0;
	else {
		printf("mouse_write: write_kbc failed\n");
		return -1;
	}
}

int mouse_read(unsigned long* val) {
	if (sys_inb(DATA, val) == 0)
		return 0;
	else{
		printf("mouse_read: sys_inb() failed\n");
		return -1;
	}
}

int mouse_enable_str_md() {
	unsigned long val;
	if (mouse_write(ENABLE_PACKETS) == 0 && mouse_read(&val) == 0)
		return 0;
	else {
		printf("mouse enable stream mode failed\n");
		return -1;
	}
}

int mouse_subscribe() {
// atualiza hook_id, passando a ser 12
	hook_id = IRQ_MOUSE;

	int bitmask = BIT(hook_id);
	if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_EXCLUSIVE, &hook_id) != 0
			|| sys_irqenable(&hook_id) != 0) {
		printf("\nMouse_subscribe() failed \n");
		return -1;
	}
	return bitmask;
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




