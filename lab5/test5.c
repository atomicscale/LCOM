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
#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

typedef struct
{ char VESASignature[4];
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
	if (!vg_exit())
		printf("Virtual address VRAM was mapped to: 0x%p", video_mem);
	else
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
	int num_video_modes;
	uint16_t *video_modes;
	VBE_VgaInfo *vmi_p = lm_alloc((sizeof vmi_p), &map);
	struct reg86u reg86;
	reg86.u.b.intno = 0x10;
	reg86.u.b.ah = 0x4F;
	reg86.u.b.al = 0x0;
	reg86.u.w.es = PB2BASE(map.phys);
	reg86.u.w.di = PB2OFF(map.phys);
	memcpy(vmi_p->VESASignature, "VBE2", sizeof("VBE2"));
	*vmi_p = *(VBE_VgaInfo*) map.virtual;
	if (memcmp(vmi_p->VESASignature, "VESA", sizeof(vmi_p->VESASignature)) != 0) {
		return 1;
	}
	void *farptr = (void *) (((vmi_p->VideoModePtr & 0xffff0000) >> 12)
			+ PB2OFF(vmi_p->VideoModePtr)
			+ ((uint32_t) map.virtual & 0xF0000000));
	lm_free(&map);
	int16_t *modes = farptr;
	num_video_modes = 0;
	size_t i;
	for (i = 0; *modes != TEMINATE; ++modes)
		num_video_modes++;
	if ((video_modes = malloc(num_video_modes * sizeof(uint16_t))) == NULL) {
		return 1;
	}
	for (i = 0, modes = farptr; i < num_video_modes; ++i, ++modes) {
		video_modes[i] = *modes;
	}
	return 0;
}

