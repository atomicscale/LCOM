#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "kbd.h"

int kbd_subscribe() {
	// atualiza hook_id, passando a ser 1
	hook_id = KBD_HOOK_BIT;
	/*The policy you should specify in sys_irqsetpolicy() is IRQ_REENABLE, so
	that the generic interrupt handler will acknowledge the interrupt. */
	if (sys_irqsetpolicy(IRQ_KBD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0)
	{
		printf("\nkbd_subscribe() failed \n");
		return -1;
	}
	else if (sys_irqenable(&hook_id) != 0))
	{
		printf("\nkbd_subscribe() failed \n");
		return -1;
	}
	else
	{
		return KBD_HOOK_BIT;
	}
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

void kbd_handler()
{
	// to complete
}
