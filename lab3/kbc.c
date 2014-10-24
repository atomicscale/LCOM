#include "kbc.h"
#include "i8042.h"

#define KBC_IO_MAX_TRIES 3

//alterar nome
int kbc_write(unsigned char cmd) {
	unsigned long stat;

	sys_inb(STAT_REG, &stat); /* assuming it returns OK */

	/* loop while 8042 input buffer is not empty */
	if ((stat & IBF) == 0) {
		sys_outb(IN_BUF, cmd); /* no args command */
		return 0;
	}
	tickdelay(micros_to_ticks(DELAY_US));
	//printf ()
	return -1;
}

int kbc_write2(unsigned char cmd, unsigned char arg) {
	unsigned long stat;
	int kbc_tries = 0;

	while (kbc_tries < KBC_IO_MAX_TRIES) {
		if (kbc_write(cmd) != 0)
			return -1;

		sys_inb(OUT_BUF, &stat); /* assuming it returns OK */
		if (stat == ACK) {
			if (kbc_write(arg) != 0)
				return -1;
			sys_inb(OUT_BUF, &stat);
			if (stat == ACK)
				return 0;
			if (stat == RESEND) {
				sys_inb(OUT_BUF, &stat);
			}
		}
	}
	return -1;
}

int kbc_read() {
	unsigned long stat, data;
	int kbc_tries = 0;
	while (kbc_tries < KBC_IO_MAX_TRIES) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else if (data == ERROR)
				return data;
			else if (data == RESEND)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		kbc_tries++;
	}
	//printf ()
	return -1;
}
