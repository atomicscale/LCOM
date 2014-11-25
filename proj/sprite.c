/** Creates a new sprite with pixmap "pic", random speeds
 * (not zero) and position (within the screen limits), and
 * draws it in memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
#include "sprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_xpm.c"

Sprite *create_sprite(char *pic[]) {
//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL)
		return NULL;
// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}
	return sp;
}

void destroy_sprite(Sprite *sp) {
	if (sp == NULL)
		return;
	free(sp->map);
	free(sp);
	sp = NULL; // hopeless: pointer is passed by value
}
int animate_sprite(Sprite *sp) {
//...
}
/* Some useful non-visible functions */
int check_collision(Sprite *sp) {
//...
}
int draw_sprite(Sprite *sp) {
	int x = 1, y = 1;
	char *map = sp->map;
	while (x <= sp->width && y <= sp->height) {
		draw_pixel(x + (int)sp->x, y + (int)sp->y, *map);
		if (x == sp->width) {
			x = 0;
			y++;
		}
		x++;
		map++;
	}
	return 0;
}


