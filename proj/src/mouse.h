#pragma once
#include "rectangle.h"
#include "bitmap.h"


typedef struct {
    int x, y;
    int xSign, ySign;
    int deltaX, deltaY;

    unsigned int counter;
    unsigned int interrupts;
    unsigned long packet[3];

    int leftButtonDown;
    int middleButtonDown;
    int rightButtonDown;

    int color;
    Bitmap* image;
    Rectangle* rect;

} Mouse;

Mouse* getMouse();

void newMouse();
void updateMouse();
void drawMouse();
void deleteMouse();
void handlerMouse();
void setMouseColor(int color);

int mouse_write(unsigned char cmd);
int mouse_read(unsigned long* val);

int mouse_subscribe();
int mouse_unsubscribe();

int rectangleInside(Rectangle* r1, Rectangle* r2);
