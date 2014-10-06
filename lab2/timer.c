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
	unsigned char read_back = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;
	sys_outb(TIMER_CTRL, read_back);

	if (timer == 0 || timer == 1 || timer == 2){
		sys_inb(TIMER_0 + timer,(unsigned long int *) st);
		return 0;
	}
	return 1;
	}



int timer_display_conf(unsigned char conf) {

	//Select counter
	if (((conf & BIT(7)) >> 7) == 0 && ((conf & BIT(6)) >> 6) == 0)
		printf("\nSelect counter: %d \n",  0);
	if (((conf & BIT(7)) >> 7) == 0 && ((conf & BIT(6)) >> 6) == 1)
		printf("\nSelect counter: %d \n",  1);
	if (((conf & BIT(7)) >> 7) == 1 && ((conf & BIT(6)) >> 6) == 0)
		printf("\nSelect counter: %d \n",  2);

	// Type of Access
	if (((conf & BIT(5)) >> 5) == 0 && ((conf & BIT(4)) >> 4) == 1)
		printf("Type of Access: %s \n", "LSB");
	if (((conf & BIT(5)) >> 5) == 1 && ((conf & BIT(4)) >> 4) == 0)
			printf("Type of Access: %s \n", "MSB");
	if (((conf & BIT(5)) >> 5) == 1 && ((conf & BIT(4)) >> 4) == 1)
			printf("Type of Access: %s \n", "LSB followed by MSB");

	//Operation Mode
	if (((conf & BIT(3)) >> 3) == 0 && ((conf & BIT(2)) >> 2) == 0 && ((conf & BIT(1)) >> 1) == 0)
		printf("Operating Mode: %d \n", 0);
	if (((conf & BIT(3)) >> 3) == 0 && ((conf & BIT(2)) >> 2) == 0 && ((conf & BIT(1)) >> 1) == 1)
		printf("Operating Mode: %d \n", 1);
	if (((conf & BIT(3)) >> 3) == 0 && ((conf & BIT(2)) >> 2) == 1 && ((conf & BIT(1)) >> 1) == 0)
		printf("Operating Mode: %d \n", 2);
	if (((conf & BIT(3)) >> 3) == 1 && ((conf & BIT(2)) >> 2) == 1 && ((conf & BIT(1)) >> 1) == 0)
		printf("Operating Mode: %d \n", 2);
	if (((conf & BIT(3)) >> 3) == 0 && ((conf & BIT(2)) >> 2) == 1 && ((conf & BIT(1)) >> 1) == 1)
		printf("Operating Mode: %d \n", 3);
	if (((conf & BIT(3)) >> 3) == 1 && ((conf & BIT(2)) >> 2) == 1 && ((conf & BIT(1)) >> 0) == 1)
		printf("Operating Mode: %d \n", 3);
	if (((conf & BIT(3)) >> 3) == 1 && ((conf & BIT(2)) >> 2) == 0 && ((conf & BIT(1)) >> 1) == 0)
		printf("Operating Mode: %d \n", 4);
	if (((conf & BIT(3)) >> 3) == 1 && ((conf & BIT(2)) >> 2) == 0 && ((conf & BIT(1)) >> 1) == 1)
		printf("Operating Mode: %d \n", 5);

	//Counting Mode
	if ((conf & BIT(0)) == 0)
		printf("Counting mode: %s \n \n",  "Binary (16 bits)");
	if ((conf & BIT(0)) == 1)
		printf("Counting mode: %s \n \n",  "BCD (4 decades)");

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
