#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include <minix/bitmap.h>
#include "mouse.h"
#include "i8042.h"

static unsigned char packet[3] = { 0, 0, 0 };
static short counter = 0;

int mouse_handler()
{
	unsigned long data;
	mouse_read(&data);
	printf("%d\n", data);


}

int test_packet(unsigned short cnt){
	int ipc_status;
	message msg;
	int receive;
	int interrupts = 0;
	int irq_set = mouse_subscribe();
	while (counter < cnt) {
			receive = driver_receive(ANY, &msg, &ipc_status);
			if (receive != 0) {
				printf("driver_receive failed with: %d", receive);
				continue;
			}

			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & irq_set) {
						mouse_handler();
						interrupts++;
					}
					break;
				default:
					break;
				}
			} else {
			}
		}
		mouse_unsubscribe();
		return 0;
}	
	
int test_async(unsigned short idle_time) {
    /* To be completed ... */
}
	
int test_config(void) {
    /* To be completed ... */
}	
	
int test_gesture(short length, unsigned short tolerance) {
    /* To be completed ... */
}
