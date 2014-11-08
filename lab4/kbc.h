#pragma once

int kbc_write(unsigned char cmd);
int kbc_write2(unsigned char cmd, unsigned char arg);
int kbc_read();
