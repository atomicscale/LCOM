#include "video_gr.h"

void *test_init(unsigned short mode, unsigned short delay) {
	
	
}

int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	if (size <= 0){
		printf ("Wrong argument: Size");
		return 1;
	}

	vg_init(0x105);
	draw_rectangle(x, y, x+size, y+size, color);
	sleep(5);
	vg_exit();
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	/* To be completed */
	
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	/* To be completed */
	
}					


