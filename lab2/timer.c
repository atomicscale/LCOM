#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
// Variaveis globais
unsigned long counter;
static int hook_id;

int timer_set_square(unsigned long timer, unsigned long freq) {
	// configura o read-back command
	unsigned char read_back = TIMER_SQR_WAVE | TIMER_BIN | (TIMER_0 + timer) | TIMER_LSB_MSB;
	// calcula a frequencia a ser utilizada
	unsigned long new_freq = TIMER_FREQ / freq;
	// tem que se passar 1 byte de cada vez, o lsb, seguido do msb
	unsigned char lsb = new_freq & 0xFF;
	unsigned char msb = new_freq >> 8;
	if (new_freq <= TIMER_FREQ && new_freq >= TIMER_MIN_FREQ)
	{
		// informa-se o controlar e de seguida passa-se o lsb e o msb
		sys_outb(TIMER_CTRL, read_back);
		sys_outb(TIMER_0 + timer, lsb);
		sys_outb(TIMER_0 + timer, msb);
		return 0;
	}
	else
	{
		printf("ERROR: freq out of range \n");
		return 1;
	}
}

int timer_subscribe_int(void ) {
	// atualiza hook_id, passando a ser 0
	hook_id = TIMER_HOOK_BIT;
	/*The policy you should specify in sys_irqsetpolicy() is IRQ_REENABLE, so
	that the generic interrupt handler will acknowledge the interrupt. */
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0 || sys_irqenable(&hook_id) != 0)
		return -1;
	else
		return TIMER_HOOK_BIT;
}

int timer_unsubscribe_int() {
	/*sys_irqrmpolicy(int *hook_id) Unsubscribes a previous interrupt notification.

	sys_irqdisable(int *hook_id) Masks an interrupt line associated
	with a previously subscribed interrupt notification.*/
	if(sys_irqrmpolicy(&hook_id) != 0 || sys_irqdisable(&hook_id) != 0)
		return -1;
		return 0;
}

void timer_int_handler() {
	// Incrementa a variavel global -> contador
	counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	// Configura o read-back command
	unsigned char read_back = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;
	// invoca sys_outb
	sys_outb(TIMER_CTRL, read_back);

	if (timer == 0 || timer == 1 || timer == 2) // 0 <= timer <= 2
	{
		// invoca sys_inb
		sys_inb(TIMER_0 + timer,(unsigned long int *) st);
		return 0;
	}
	return 1;
	}



int timer_display_conf(unsigned char conf) {
	//Output - indica se está ativo
	 printf("\nOutput: %d \n",  (conf & BIT(7)) >> 7);

	//Null Count - indica se está à espera de um novo valor
	 printf("Null Count: %d \n",  (conf & BIT(6)) >> 6);

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
	//chama a função timer_set_square com timer0
	if (!timer_set_square(0, freq))
		return 0;
	else
		return 1;
}

int timer_test_int(unsigned long time) {

	int ipc_status;
	message msg;
	// usado para evitar chamar a função driver_receive várias vezes
	int receive;
	counter = 0; // Inicializa o contador
	timer_subscribe_int();
	// condição verdadeira, sempre que é multiplo de 60( de 60 em 60 segundos )
	while(counter <= time*60) {
	/* Get a request message. */
			receive = driver_receive(ANY, &msg, &ipc_status);
		if (receive != 0 ) {
			printf("driver_receive failed with: %d", receive);
				continue;
	}
	if (is_ipc_notify(ipc_status)) { /* received notification */
		switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & BIT(TIMER_HOOK_BIT)) { /* subscribed interrupt */
						timer_int_handler();  /* process it */
	                    if(counter % 60 == 0){
	                    	printf("\tSucesso\n");
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
	timer_unsubscribe_int();

	return 0;
}

int timer_test_config(unsigned long timer) {
	unsigned char st;
	// Confirma se nenhuma das funções chamadas dá erro
	if (timer_get_conf(timer, &st) != 0)
	{
		printf("ERROR: Timer ranges from 0 to 2 \n");
		return 1;
	}
	else
	{
		// Só imprime se get_config não der erro
		if (timer_display_conf(st) != 0)
			return 1;
		else
			return 0;
	}

}
