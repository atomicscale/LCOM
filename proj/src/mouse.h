#pragma once



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

} Mouse;



void newMouse();
void updateMouse();
void drawMouse();
void deleteMouse();
void handlerMouse();

int mouse_write(unsigned char cmd);
int mouse_read(unsigned long* val);

int mouse_subscribe();
int mouse_unsubscribe();
