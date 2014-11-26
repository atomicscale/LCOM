#pragma once

int mouse_write(unsigned char cmd);
int mouse_read(unsigned long* val);


int mouse_subscribe();

int mouse_unsubscribe();
