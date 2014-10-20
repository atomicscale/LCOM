#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define OUT_BUF 0x60

int kbd_write (char data){
	while( 1 ) {
			sys_inb(STAT_REG, &stat); /* assuming it returns OK */
					/* loop while 8042 input buffer is not empty */
				if( (stat & IBF) == 0 ) {
						sys_outb(KBC_CMD_REG, cmd); /* no args command */
							return 0;
					}
					delay(WAIT_KBC);
				}
}

int kbd_read (){

	while( 1 ) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
			/* loop while 8042 output buffer is empty */
		if( stat & OBF ) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
		if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
			return data;
		else
			return -1;
		}
		delay(WAIT_KBC);
	}
}
