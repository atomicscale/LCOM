#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"
#include "i8042.h"
#include "timer.h"

//Variaveis globais
unsigned long counter;
// ass se for 0, utiliza o handler em C, outro valor executa em Assembly
int kbd_test_scan(unsigned short ass) {
	int ipc_status;
	message msg;
	int validation = 0;

	// usado para evitar chamar a função driver_receive várias vezes
	int receive;

	// kbd_subscribe() já verifica se não há problemas.
	int irq_set = kbd_subscribe();

	//Quando tecla esc é libertada, pára
	while (!validation) {
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
					if (ass == 0)
						validation = kbd_handler_c(); /* process it */
					else
						validation = kbd_handler_asm();
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	// kbd_unsubscribe_int() já verifica se não há problemas.
	kbd_unsubscribe();
	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *leds) {
	int i = 0;
	unsigned char mask = 0;
	while (i < n) {
		mask ^= BIT(leds[i]);
		kbc_write2(SW_LEDS, mask);
		i++;
		// espera um segundo entre cada elemento do array, utilizando o timer
		timer_test_int(1);
	}
	return 0;
}
int kbd_test_timed_scan(unsigned short n) {
	int ipc_status;
	message msg;
	int validation = 0;
	int counter = 0;

	// usado para evitar chamar a função driver_receive várias vezes
	int receive;

	//subscribe() já verifica se não há problemas.
	int irq_set1 = kbd_subscribe();
	int irq_set2 = timer_subscribe_int();

	//Para quando a tecla ESC é libertada, ou quando passam n segundos sem input
	while (!validation) {
		/* Get a request message. */
		receive = driver_receive(ANY, &msg, &ipc_status);
		if (receive != 0) {
			printf("driver_receive failed with: %d", receive);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set1) { /* subscribed interrupt */
					validation = kbd_handler_c(); /* process it */
					counter = 0;
				}
				if (msg.NOTIFY_ARG & irq_set2) { /* subscribed interrupt */
					counter++; /* process it */
					if (counter == n*60)
					{
						printf("\n\tkbd_test_scan() stopped, press ENTER to continue!\n");
						validation = 1;
					}
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
	timer_unsubscribe_int();
	return 0;
}

