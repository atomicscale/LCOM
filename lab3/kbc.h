#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define OUT_BUF 0x60
#define KBC_IO_MAX_TRIES 3

int kbc_write(unsigned char cmd);
int kbc_read();
