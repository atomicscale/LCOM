#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "video_gr.h"
#include "i8042.h"

void* vg_init(unsigned short mode) {
	unsigned int vram_size;
	struct mem_range mr;
	vbe_mode_info_t vmi_p;
	struct reg86u reg86;

	reg86.u.b.intno = BIOS_VIDEO;
	reg86.u.b.ah = VBE;
	reg86.u.b.al = SET_VBE;
	reg86.u.w.bx = BIT(LINEAR_BIT) | mode;

	if (sys_int86(&reg86)) {
		printf("vg_init()::bios call didn't return 0\n");
		return NULL;
	}

	if (vbe_get_mode_info(mode, &vmi_p) == -1) {
		printf("vg_init()::failed in vbe_get_mode_info");
		return NULL;
	}

	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;
	if (bits_per_pixel / 8 > 0)
		bytes_per_pixel = bits_per_pixel / 8;
	else
		bytes_per_pixel = 1;
	vram_size = h_res * v_res * bytes_per_pixel;

	int r;

	/* Allow memory mapping */

	mr.mr_base = vmi_p.PhysBasePtr;
	mr.mr_limit = mr.mr_base + vram_size;

	if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) {
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);
		return NULL;
	}

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

	if (video_mem == MAP_FAILED) {
		panic("video_txt couldn't map video memory");
		return NULL;
	}

	return video_mem;

}

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = BIOS_VIDEO; /* BIOS video services */

	reg86.u.b.ah = VIDEO; /* Set Video Mode function */
	reg86.u.b.al = SET_TEXT; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else {
		return 0;
	}
}

void draw_pixel(unsigned int x, unsigned int y, char color) {
	if (x < H_RES && y < V_RES) {
		*(video_mem + (y << 10) + x) = color;

	} else if (x < 0 || y < 0)
		printf("Wrong values for x or y");
}

int draw_rectangle(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, char color) {

	if (xf > H_RES) {
		xf = H_RES;
		xi = H_RES - 10;
	}

	if (yf > V_RES) {
		yf = V_RES;
		yi = V_RES - 10;

	}

	if (xf < xi) {
		SWAP(xi, xf);
	}

	if (yf < yi) {
		SWAP(yi, yf);
	}

	unsigned short x, y;

	for (x = xi; x < xf; x++) {
		for (y = yi; y < yf; y++) {
			draw_pixel(x, y, color);
		}
	}
}

char* getGraphicsBuffer(){
	return video_mem;
}
