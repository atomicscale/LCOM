#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"
#include "i8042.h"

static int hook_id;

int kbd_subscribe() {
	// atualiza hook_id, passando a ser 1
	hook_id = IRQ_KBD;

	int bitmask = BIT(hook_id);

	/*The policy you should specify in sys_irqsetpolicy() is IRQ_REENABLE | IRQ_REENABLE*/
	if (sys_irqsetpolicy(IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK
			|| sys_irqenable(&hook_id) != OK) {
		printf("\nkbd_subscribe() failed \n");
		return -1;
	}

	return bitmask;

}

int kbd_unsubscribe() {
	/*sys_irqrmpolicy(int *hook_id) Unsubscribes a previous interrupt notification.

	 sys_irqdisable(int *hook_id) Masks an interrupt line associated
	 with a previously subscribed interrupt notification.*/

	if (sys_irqdisable(&hook_id) != 0) {
		printf("\nkbd_unsubscribe failed \n");
		return 1;
	}

	if (sys_irqrmpolicy(&hook_id) != 0) {
		printf("\nkbd_unsubscribe failed \n");
		return 1;
	}

	return 0;
}

int kbd_handler_c() {
	unsigned long long scan_code = kbc_read();
	if (scan_code != -1) {
		if (scan_code == KEY_UP(KEY_ESC)) {
			printf("\n\tkbd_test_scan() stopped, press ENTER to continue!\n");
			return 1;
		} else {
			if (scan_code == 0xe0)
				scan_code = (scan_code << 8) | kbc_read();
			// Pause key, 6bytes
			else if (scan_code == 0xe1) {
				int i;
				for (i = 0; i < 5; ++i)
					scan_code = (scan_code << 8) | kbc_read();
				printf("\tMakecode: 0x");
				for (i = 5; i >= 0; --i)
					printf("%X", (scan_code >> 8 * i) & 0xFF);
				printf("\n");
				return 0;
			}
			if (scan_code & 0x80)
				printf("\tBreakcode: 0x%X\n", scan_code);
			else
				printf("\tMakecode: 0x%X\n", scan_code);
			return 0;
		}
	} else
		return 0;
}

int kbd_handler_asm()
{
	unsigned long long scan_code = handler_asm();
	if (scan_code != -1) {
		if (scan_code == KEY_UP(KEY_ESC)) {
			printf("\n\tkbd_test_scan() stopped, press ENTER to continue!\n");
			return 1;
		} else {
			if (scan_code == 0xe0)
			{
				scan_code = handler_asm();
				if (scan_code & 0x80)
					printf("\tBreakcode: 0xE0%X\n", scan_code);
				else
					printf("\tMakecode: 0xE0%X\n", scan_code);
				return 0;
			}
			else
			{
			if (scan_code & 0x80)
				printf("\tBreakcode: 0x%X\n", scan_code);
			else
				printf("\tMakecode: 0x%X\n", scan_code);
			return 0;
			}
		}
	} else
		return 0;
}
