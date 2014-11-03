#include "kbc.h"
#include "i8042.h"

int kbc_write(unsigned char cmd, unsigned char arg) {
	unsigned long stat;
	int kbc_tries = 0;

	while (kbc_tries < KBC_IO_MAX_TRIES) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			sys_outb(IN_BUF, cmd);
		} else {
			return -1;
		}
		sys_inb(OUT_BUF, &stat);
		if (stat == ACK) {
			sys_inb(OUT_BUF, &stat);
			if (stat == ACK)
				return 0;
			if (stat == RESEND) {
				sys_inb(OUT_BUF, &stat);
			}
		}
		tickdelay(micros_to_ticks(DELAY_US));
		kbc_tries++;
	}
	return -1;
}

int kbc_read() {
	unsigned long stat, data;
	int kbc_tries = 0;
	while (kbc_tries < KBC_IO_MAX_TRIES) {
		sys_inb(STAT_REG, &stat);
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data);
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
	return -1;
}
