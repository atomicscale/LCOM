#include "kbc.h"

int kbc_write(unsigned char cmd) {
        unsigned long stat;
        int kbc_tries = 0;

        while (kbc tries < KBC_IO_MAX_TRIES ) {
                sys_inb(STAT_REG, &stat); /* assuming it returns OK */

                /* loop while 8042 input buffer is not empty */
                if ((stat & IBF) == 0) {
                        sys_outb(KBC_CMD_REG, cmd); /* no args command */
                        return 0;
                }
                delay(WAIT_KBC);
                kbc_tries++;
        }
        //printf ()
        return -1;
}

int kbc_read() {

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
                delay(WAIT_KBC);
                kbc_tries++;
        }
        //printf ()
        return -1;
}
