#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"

unsigned long counter;
static int hook_id;

int timer_set_square(unsigned long timer, unsigned long freq) {
	unsigned char read_back = TIMER_SQR_WAVE | TIMER_BIN | (TIMER_0 + timer) | TIMER_LSB_MSB;
	unsigned long divisor = TIMER_FREQ / freq;
	if (divisor < 0xFFFF)
	{
		sys_outb(TIMER_CTRL, read_back);
		sys_outb(TIMER_0 + timer, divisor & 0xFF);
		sys_outb(TIMER_0 + timer, divisor >> 8);
		return 0;
	}
	else
		return 1;
}

int timer_subscribe_int(void ) {
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0 || sys_irqenable(&hook_id) != 0) //The policy you should specify in sys_irqsetpolicy() is IRQ_REENABLE, so that the generic interrupt handler will acknowledge the interrupt,
		return -1;
		//return something;
}

int timer_unsubscribe_int() {
	if(sys_irqrmpolicy(&hook_id) != 0 || sys_irqdisable(&hook_id) != 0) //sys_irqrmpolicy(int *hook_id) Unsubscribes a previous interrupt notification,
		return -1;																//by specifying a pointer to thehook_id returned by the kernel																	//sys_irqdisable(int *hook_id) Masks an interrupt line associated with a previously subscribed interrupt notification,
		return 0;										 						//by specifying a pointer to the hook_id returned by the kernel
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	unsigned char read_back = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;
	sys_outb(TIMER_CTRL, read_back);

	if (timer == 0 || timer == 1 || timer == 2){
		sys_inb(TIMER_0 + timer,(unsigned long int *) st);
		return 0;
	}
	return 1;
	}



int timer_display_conf(unsigned char conf) {
	//Output - indica se está ativo ou não
	 printf("\nOutput: %d \n",  (conf & BIT(7)) >> 7):

	//Null Count - indica se está à espera de um novo valor ou não
	 printf("\nNull Count: %d \n",  (conf & BIT(6)) >> 6):

	// Type of Access
	if (((conf & BIT(5)) >> 5) == 0 && ((conf & BIT(4)) >> 4) == 1)
		printf("Type of Access: %s \n", "LSB");
	if (((conf & BIT(5)) >> 5) == 1 && ((conf & BIT(4)) >> 4) == 0)
			printf("Type of Access: %s \n", "MSB");
	if (((conf & BIT(5)) >> 5) == 1 && ((conf & BIT(4)) >> 4) == 1)
			printf("Type of Access: %s \n", "LSB followed by MSB");

	//Operation Mode
	if (((conf & BIT(3)) >> 3) == 0 && ((conf & BIT(2)) >> 2) == 0 && ((conf & BIT(1)) >> 1) == 0)
		printf("Operating Mode: %s \n", "0 - Interrupt on terminal count");
	if (((conf & BIT(3)) >> 3) == 0 && ((conf & BIT(2)) >> 2) == 0 && ((conf & BIT(1)) >> 1) == 1)
		printf("Operating Mode: %s \n", "1 - Hardware retriggerable one-shot");
	if (((conf & BIT(3)) >> 3) == 0 && ((conf & BIT(2)) >> 2) == 1 && ((conf & BIT(1)) >> 1) == 0)
		printf("Operating Mode: %s \n", "2 - Rate generator");
	if (((conf & BIT(3)) >> 3) == 1 && ((conf & BIT(2)) >> 2) == 1 && ((conf & BIT(1)) >> 1) == 0)
		printf("Operating Mode: %s \n", "2 - Rate generator");
	if (((conf & BIT(3)) >> 3) == 0 && ((conf & BIT(2)) >> 2) == 1 && ((conf & BIT(1)) >> 1) == 1)
		printf("Operating Mode: %s \n", "3 - Square wave mode");
	if (((conf & BIT(3)) >> 3) == 1 && ((conf & BIT(2)) >> 2) == 1 && ((conf & BIT(1)) >> 0) == 1)
		printf("Operating Mode: %s \n", "3 - Square wave mode");
	if (((conf & BIT(3)) >> 3) == 1 && ((conf & BIT(2)) >> 2) == 0 && ((conf & BIT(1)) >> 1) == 0)
		printf("Operating Mode: %s \n", "4 - Software triggered strobe");
	if (((conf & BIT(3)) >> 3) == 1 && ((conf & BIT(2)) >> 2) == 0 && ((conf & BIT(1)) >> 1) == 1)
		printf("Operating Mode: %s \n", "5 - Hardware triggered strobe (retriggerable)");


	//Counting Mode
	if ((conf & BIT(0)) == 0)
		printf("Counting mode: %s \n \n",  "Binary (16 bits)");
	if ((conf & BIT(0)) == 1)
		printf("Counting mode: %s \n \n",  "BCD (4 decades)");


	return 0;
}

int timer_test_square(unsigned long freq) {
	if (!timer_set_square(0, freq))
		return 0;
	else
		return 1;
}

int timer_test_int(unsigned long time) {

	int ipc_status;
	message msg;
	
	while( 1 ) { /* You may want to use a different condition */
	/* Get a request message. */
	if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
		printf("driver_receive failed with: %d", r);
		continue;
	}
	if (is_ipc_notify(ipc_status)) { /* received notification */
		switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
	                    ...   /* process it */
				}
						break;
				default:
					break; /* no other notifications expected: do nothing */
				}
	} else { /* received a standard message, not a notification */
	/* no standard messages expected: do nothing */
	}
}

	return 1;
}

int timer_test_config(unsigned long timer) {
	unsigned char st;
	int n1, n2;
	n1 = timer_get_conf(timer, &st);
	n2 = timer_display_conf(st);
	if (n1 == 0 && n2 == 0 ){
		return 0;
	}
	else
		return 1;
}
