#pragma once

int mouse_write(unsigned char cmd);
int mouse_read(unsigned long* val);

//enable stream mode
int mouse_enable_str_md();

int mouse_subscribe();

int mouse_unsubscribe();
