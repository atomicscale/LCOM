#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "kbd.h"

//Variaveis globais
unsigned long counter;
static int hook_id;




int kbd_test_scan(unsigned short ass); {
	if (ass == 0)
	{
		int ipc_status;
		message msg;
		// usado para evitar chamar a função driver_receive várias vezes
		int receive;
		counter = 0; // Inicializa o contador
		// kbd_subscribe() já verifica se não há problemas.
		kbd_subscribe();
		// condição verdadeira, sempre que é multiplo de 60( de 60 em 60 segundos )
		while(kbd_scancode) {
		/* Get a request message. */
			receive = driver_receive(ANY, &msg, &ipc_status);
			if (receive != 0 ) {
				printf("driver_receive failed with: %d", receive);
					continue;
			}
			if (is_ipc_notify(ipc_status)) { /* received notification */
				switch (_ENDPOINT_P(msg.m_source)) {
					case HARDWARE: /* hardware interrupt notification */
						if (msg.NOTIFY_ARG & BIT(KBD_HOOK_ID)) { /* subscribed interrupt */
							kbd_handler();  /* process it */
		                    if(counter % 60 == 0){
		                    	printf("\tSucess\n");
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
		return 0;
	}
	else
	{
		//completar com código assembly
	}
}
int kbd_test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}
int kbd_test_timed_scan(unsigned short n) {
    /* To be completed */
}

