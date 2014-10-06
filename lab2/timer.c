#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "lab2.h"

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
	unsigned long read_back = TIMER_RB_CMD | TIMER_LSB_MSB
	int sys_outb(TIMER_CTRL, read_back);
	
		swich(timer)
	{
			case 0:
				int sys_inb(TIMER_RB_SEL(timer), *st)
			case 1:
				int sys_inb(TIMER_RB_SEL(timer), *st)
			case 2:
				int sys_inb(TIMER_RB_SEL(timer), *st)

	}
		return 0;

			return 1;
}

int timer_display_conf(unsigned char conf) {
	


	return 1;
}

int timer_test_square(unsigned long freq) {
	
	return 1;
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {
	unsigned char st;
	if (timer_get_conf (timer, *st) != 0){
		return timer_get_conf(timer, *st);
	}
	return timer_display_conf(st);
	
}
