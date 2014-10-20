#include "kbc.h"
#include "i8042.h"

#define KBC_IO_MAX_TRIES 3

int kbc_write(unsigned char cmd) {
        unsigned long stat;
        int kbc_tries = 0;

        while (kbc_tries < KBC_IO_MAX_TRIES ) {
                sys_inb(STAT_REG, &stat); /* assuming it returns OK */

                /* loop while 8042 input buffer is not empty */
                if ((stat & IBF) == 0) {
                        sys_outb(KBC_CMD_REG, cmd); /* no args command */
                        return 0;
                }
                tickdelay(micros_to_ticks(DELAY_US));
                kbc_tries++;
        }
        //printf ()
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
                        else
                                return -1;
                }
                tickdelay(micros_to_ticks(DELAY_US));
                kbc_tries++;
        }
        //printf ()
        return -1;
}
