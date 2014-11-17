#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "video_gr.h"

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

#define BIOS_VIDEO 0x10
#define VBE 0x4F
#define VIDEO 0x00
#define SET_VBE 0x02
#define SET_TEXT 0x03
#define BIT(n) (0x1 << (n))
#define LINEAR_BIT 14

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


void* vg_init(unsigned short mode)
{
    unsigned int vram_size;
    struct mem_range mr;
    vbe_mode_info_t vmi_p;
    struct reg86u reg86;

    reg86.u.b.intno = BIOS_VIDEO;
    reg86.u.b.ah = VBE;
    reg86.u.b.al = SET_VBE;
    reg86.u.w.bx = BIT(LINEAR_BIT) | mode;

	if (sys_int86(&reg86) != OK) {
		printf("vg_init()::bios call didn't return 0\n");
		return NULL;
	}

    h_res = H_RES;
    v_res = V_RES;
    bits_per_pixel = BITS_PER_PIXEL;

    int r;

    /* Allow memory mapping */

    mr.mr_base = (phys_bytes) VRAM_PHYS_ADDR;
    mr.mr_limit = mr.mr_base + (h_res * v_res);

      if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0)
    	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

      /* Map memory */

      video_mem = vm_map_phys(SELF, (void *)mr.mr_base, h_res * v_res);

      if(video_mem == MAP_FAILED)
    	  panic("video_txt couldn't map video memory");

      return video_mem;
}



int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = BIOS_VIDEO; /* BIOS video services */

  reg86.u.b.ah = VIDEO;    /* Set Video Mode function */
  reg86.u.b.al = SET_TEXT;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

void draw_pixel(int x, int y, char color) {
	if (x < H_RES && y < V_RES) {
		*(video_mem + (y << 10) + x) = color;
	}
	else if (x < 0 || y < 0)
		printf("Wrong values for x or y");
}

int draw_rectangle(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, char color) {

	if (xi > H_RES || xf > H_RES || yi > V_RES || yf > V_RES) {
		return 1;
	}

	if (xf < xi) {
		SWAP(xi, xf);
	}

	if(yf < yi){
		SWAP(yi, yf);
	}

unsigned short x, y;

	for (x = xi; x < xf; x++) {
		for (y = yi; y < yf; y++) {
			draw_pixel(x, y, color);
		}
	}
}
