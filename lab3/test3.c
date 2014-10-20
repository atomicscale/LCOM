#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"
#include "i8042.h"

//Variaveis globais
unsigned long counter;

int kbd_test_scan(unsigned short ass) {
	unsigned long scancode = 0;
	int ipc_status;
	message msg;

	// usado para evitar chamar a função driver_receive várias vezes
	int receive;

	// kbd_subscribe() já verifica se não há problemas.
	int irq_set = kbd_subscribe();

	// condição verdadeira, sempre que é multiplo de 60( de 60 em 60 segundos )
	while (scancode != 0x81) {
		/* Get a request message. */
		receive = driver_receive(ANY, &msg, &ipc_status);
		if (receive != 0) {
			printf("driver_receive failed with: %d", receive);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					scancode = kbc_read();
					kbd_handler_c(scancode); /* process it */
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	// unsubscribe_int() já verifica se não há problemas.
	kbd_unsubscribe();
	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *leds) {

}
int kbd_test_timed_scan(unsigned short n) {
	/* To be completed */
}

