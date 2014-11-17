#include "video_gr.h"

void *test_init(unsigned short mode, unsigned short delay) {
	vg_init(mode);
	vg_exit();
}

int test_square(unsigned short x, unsigned short y,short size, unsigned long color) {

	if (size <= 0){
		printf ("Wrong argument: Size");
		return 1;
	}

	else{
		vg_init(0x105);
		draw_rectangle(x, y, x+size, y+size, color);
		sleep(5);
		vg_exit();
		return 0;
	}
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, char color) {
	vg_init(0x105);
	/* Midpoint Line algorithm */
	int dx,dy,d,incry,incre,incrne,slopegt1=0;
		dx=abs(xi-xf);dy=abs(yi-yf);
		if(dy>dx)
		{
			SWAP(xi,yi);
			SWAP(xf,yf);
			SWAP(dx,dy);
			slopegt1=1;
		}
		if(xi>xf)
		{
			SWAP(xi,xf);
			SWAP(yi,yf);
		}
		if(yi>yf)
			incry=-1;
		else
			incry=1;
		d=2*dy-dx;
		incre=2*dy;
		incrne=2*(dy-dx);
		while(xi<xf)
		{
			if(d<=0)
				d+=incre;
			else
			{
				d+=incrne;
				yi+=incry;
			}
			xi++;
			if(slopegt1)
				draw_pixel(yi,xi,color);
			else
				draw_pixel(xi,yi,color);
		}
		sleep(5);
	vg_exit();
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	/* To be completed */
	
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	/* To be completed */
	
}					


