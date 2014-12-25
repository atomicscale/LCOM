#include "utilities.h"

#define GFXMODE "0x114"

const char* getImagePath(const char* image){
	char num[256];
	sprintf(num, "/home/lcom/lcom1415-t3g07/proj/rec/images/%s-%s.bmp", image, GFXMODE);

	char* str = (char*)malloc(256);
	strcpy(str, num);

	return str;
}


