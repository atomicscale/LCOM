#include "video_gr.h"
#include "keyboard.h"
#include "i8042.h"
#include "timer.h"
#include "read_xpm.c"
#include "pixmap.h"
#include "sprite.h"
#include "vbe.h"
#include "lmlib.h"
#include <stdint.h>
#include <machine/int86.h>

#define CEILING(x,y) (((x) + (y) - 1) / (y))
#define TEMINATE 0xFFFF

// http://minirighi.sourceforge.net/html/group__KLowLevelMemoryManager.html
#define FP_SEG(fp)   ((fp) >> 16)
#define FP_OFF(fp)   ((fp) & 0xffff)
#define FP_TO_LINEAR(seg, off)   (((((uint16_t) (seg)) << 4) + ((uint16_t) (off))))

typedef struct {
	char VESASignature[4];
	short VESAVersion;
	char *OemStringPtr;
	uint32_t Capabilities;
	phys_bytes VideoModePtr;
	short TotalMemory;
	//VBE 2.0
	short OemSoftwareRev;
	char *OemVendorNamePtr;
	char *OemProductNamePtr;
	char *OemProductRevPtr;
	char reserved[222];
	char OemData[256];
}__attribute__((packed)) VBE_VgaInfo;

void *test_init(unsigned short mode, unsigned short delay) {
	vg_init(mode);
	wait_seconds(delay);
	if (vg_exit())
		printf("Virtual address VRAM was mapped to: NULL");
}

int test_square(unsigned short x, unsigned short y, short size,
		unsigned long color) {

	if (size <= 0) {
		printf("Wrong argument: Size");
		return 1;
	}

	else {
		vg_init(GRAPHIC_MODE);
		draw_rectangle(x, y, x + size, y + size, color);
		kbd_wait_key(KEY_ESC);
		vg_exit();
		return 0;
	}
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, char color) {
	vg_init(GRAPHIC_MODE);

	int dx = abs(xf - xi), sx = xi < xf ? 1 : -1;
	int dy = abs(yf - yi), sy = yi < yf ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;
	// algorithm from http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
	for (;;) {
		draw_pixel(xi, yi, color);
		if (xi == xf && yi == yf)
			break;
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			xi += sx;
		}
		if (e2 < dy) {
			err += dx;
			yi += sy;
		}
	}
	//
	kbd_wait_key(KEY_ESC);
	vg_exit();
	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	if (xi > H_RES || yi > V_RES) {
		printf("ERROR! xi > H_RES or yi > V_RES\n");
		return 1;
	}
	vg_init(GRAPHIC_MODE);
	Sprite *sp = create_sprite(xpm);
	sp->x = xi;
	sp->y = yi;
	draw_sprite(sp);
	kbd_wait_key(KEY_ESC);
	vg_exit();
	return 0;

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {
	if (xi > H_RES || yi > V_RES) {
		printf("ERROR! xi > H_RES or yi > V_RES\n");
		return 1;
	}
	if (hor == 0) {
		if (xi + delta > H_RES) {
			printf("ERROR! xi + delta > H_RES\n");
			return 1;
		}
	} else {
		if (yi + delta > V_RES) {
			printf("yi + delta > V_RES\n");
			return 1;
		}
	}
	vg_init(GRAPHIC_MODE);
	Sprite *sp = create_sprite(xpm);
	sp->x = xi;
	sp->y = yi;
	// inc = delta / (time * 60);
	//Corrigir este pormenor
	int inc = CEILING(delta, (time * 60));
	draw_sprite(sp);
	int ipc_status;
	message msg;
	int receive;
	int validation = 0;
	unsigned long long scan_code;
	int irq_set_timer = timer_subscribe_int();
	int irq_set_kbd = kbd_subscribe();
	while (!validation) {
		receive = driver_receive(ANY, &msg, &ipc_status);
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer) {
					clean_xpm(sp->x, sp->y, sp->width, sp->height);
					if (sp->x > xi + delta || sp->y > yi + delta)
						validation = 1;
					if (hor == 0) {
						sp->x = sp->x + inc;
					} else {
						sp->y = sp->y + inc;
					}
					draw_sprite(sp);
				}
				if (msg.NOTIFY_ARG & irq_set_kbd) {
					scan_code = kbc_read();
					if (scan_code != -1) {
						if (scan_code == KEY_UP(KEY_ESC))
							validation = 1;
					}
				}
				break;
			default:
				break;
			}
		}
	}
	timer_unsubscribe_int();
	kbd_unsubscribe();
	kbd_wait_key(KEY_ESC);
	vg_exit();
	return 0;
}

int test_controller() {
	mmap_t map;
	char *virtual = lm_init();
	VBE_VgaInfo *vga_info = lm_alloc(sizeof(vga_info), &map);
	vbe_get_controler_info(map.phys);
	memcpy(vga_info->VESASignature, "VBE2", sizeof("VBE2"));
	unsigned short *video_modes =
			FP_TO_LINEAR(FP_SEG(vga_info->VideoModePtr), FP_OFF(vga_info->VideoModePtr))
					+ virtual;
	while (*video_modes != TEMINATE) {
		printf("0x%X, ", *video_modes);
		video_modes++;
	}
	printf("\n\tVRAM SIZE: %d KB\n", 64*vga_info->TotalMemory);
	lm_free(&map);
	return 0;
}
