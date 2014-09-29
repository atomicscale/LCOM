#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

#include "math.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {
  int i;
  char *ptr;
  ptr = video_mem;
  
  for (i=0; i < scr_width*scr_lines; i++, ptr++){
	  	  *ptr = ch;
	  	  ptr++;
	  	  *ptr = attr;
  }
  
}

void vt_blank() {

	memset(video_mem, 0, scr_width*scr_lines*2);

}

int vt_print_char(char ch, char attr, int r, int c) {
	char *ptr;
	ptr = video_mem;
	ptr = ptr+c*r*2;
	*ptr = ch;
	ptr++;
	*ptr = attr;

	return 0;
  
}

int vt_print_string(char *str, char attr, int r, int c) {

	char *ptr;
	ptr = video_mem;
	ptr = ptr+c*r*2;
	int i = 0;
	while (str[i] != 0)
	{
		*ptr = str[i];
		ptr++;
		*ptr = attr;
		ptr++;
		i++;
	}
	return 0;
}

int vt_print_int(int num, char attr, int r, int c) {

	char *ptr;
	ptr = video_mem;
	ptr = ptr+c*r*2;
	int firstDigit, numberOfDigits = 0, power, num_aux, module;
	if(num<0)
	{
		*ptr = '-';
		ptr++;
		*ptr = attr;
		ptr++;
	}
	num_aux = abs(num);
	while(num_aux)
	{
		num_aux=num_aux/10;
		numberOfDigits++;
	}
	numberOfDigits--;
	num_aux = abs(num);
	while (numberOfDigits>=0)
	{
		power = pow(10,numberOfDigits);
		firstDigit = num_aux / power;
		*ptr = firstDigit + '0';
		num_aux =  num_aux % power;
		numberOfDigits--;
		ptr++;
		*ptr = attr;
		ptr++;
	}
	return 0;

}


int vt_draw_frame(int width, int height, char attr, int r, int c) {
	char *ptr;
	ptr = video_mem;
	ptr = ptr+c*r*2;
	int i, j;
	for (i=1; i<=height; i++)
	{
		for (j=1; j<=width; j++)
		{
			if (i == 1 || i == height || j ==1 || j == width)
			{
				*ptr = '*';
				ptr++;
				*ptr = attr;
				ptr++;
			}
			else
				ptr+=2;
		}
		ptr += (scr_width-width)*2;
	}
	return 0;
}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
