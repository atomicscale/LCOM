#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"


int timer_set_square(unsigned long timer, unsigned long freq) {

	return 1;
}

int timer_subscribe_int(void ) {

	return 1;
}

int timer_unsubscribe_int() {

	return 1;
}

void timer_int_handler() {

}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	unsigned long read_back = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;
	sys_outb(TIMER_CTRL, read_back);

	if (timer == 0 || timer == 1 || timer == 2){
		sys_inb(TIMER_0+ timer, st);
		return 0;
	}
	return 1;
	}

int timer_display_conf(unsigned char conf) {
	printf("Bit 7: %d \n",  (conf & BIT(7)) >> 7);
	printf("Bit 6: %d \n",  (conf & BIT(6)) >> 6);
	printf("Bit 5: %d \n",  (conf & BIT(5)) >> 5);
	printf("Bit 4: %d \n",  (conf & BIT(4)) >> 4);
	printf("Bit 3: %d \n",  (conf & BIT(3)) >> 3);
	printf("Bit 2: %d \n",  (conf & BIT(2)) >> 2);
	printf("Bit 1: %d \n",  (conf & BIT(1)) >> 1);
	printf("Bit 0: %d \n"),  (conf & BIT(0));

		return 0;
}

int timer_test_square(unsigned long freq) {
	
	return 1;
}

int timer_test_int(unsigned long time) {
	
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
