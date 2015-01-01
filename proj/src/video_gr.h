#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H


#include "rectangle.h"


/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8


#define GRAPHIC_MODE 0x114
#define BIOS_VIDEO 0x10
#define VBE 0x4F
#define VIDEO 0x00
#define SET_VBE 0x02
#define SET_TEXT 0x03
#define LINEAR_BIT 14

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */
static char *buffer;

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel;
static unsigned int vram_size;


#define SWAP(X,Y) { \
       int temp = X ; \
       X = Y ; \
       Y = temp ; \
    }

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */

void draw_pixel(unsigned int x, unsigned int y, int color, char* buf);

int draw_rectangle(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, int color, char* buf);

void draw_circle(int x0, int y0, int radius, int color, char* buf);

void see_circle(int x0, int y0, int radius, int color, char* buf);

char* getGraphicsBuffer();

void flipScreen();

unsigned getH_res();
unsigned getV_res();
void drawMaze();
void draw_rectangle2(Rectangle* rectangle, int color, char* buf);
int rgb(unsigned char r, unsigned char g, unsigned char b);

#endif /* __VIDEO_GR_H */
