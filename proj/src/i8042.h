#pragma once

#include <minix/sysutil.h>
#include <minix/syslib.h>

#define BIT(n) (0x01<<(n))

#define OBF BIT(0)
#define IBF BIT(1)
#define TO_ERR BIT(6)
#define PAR_ERR BIT(7)
#define AUX BIT(5)

#define IRQ_KBD 1

#define STAT_REG 0x64
#define CMD_REG 0x64
#define IN_BUF 0x60
#define OUT_BUF 0x60
#define DATA 0x60
#define KBC_IO_MAX_TRIES 3

#define SW_LEDS 0xED

#define RESEND 0xFE
#define ERROR 0xFC
#define ACK 0xFA

//Mouse
#define WRITE_BYTE 0xD4
#define ENABLE_PACKETS 0xF4
#define MOUSE_STATUS 0xE9
#define STREAM_MODE 0xEA
#define DISABLE_STREAM 0xF5
#define IRQ_MOUSE 12
#define MOUSE_HOOK_ID 12
#define DELAY_US    20000


#define LEFT_B(b) ((b) & 1)
#define RIGHT_B(b) (((b) >> 1) & 1)
#define MIDDLE_B(b) (((b) >> 2) & 1)
#define X_OVERFLOW(b) (((b) >> 6) & 1)
#define Y_OVERFLOW(b) (((b) >> 7) & 1)
#define X_NEGATIVE(b) (((b) >> 4) & 1)
#define Y_NEGATIVE(b) (((b) >> 5) & 1)
#define X_VAL(packet) (short)(packet[1] | (X_NEGATIVE(packet[0]) ? 0xFF : 0) << 8)
#define Y_VAL(packet) (short)(packet[2] | (Y_NEGATIVE(packet[0]) ? 0xFF : 0) << 8)

//TEST_CONFIG
#define MODE(b) !(((b) >> 6) & 1)
#define ENABLE(b) !(((b) >> 5) & 1)
#define SCALING(b) (((b) >> 4) & 1)
#define LEFT(b) !(((b) >> 3) & 1)
#define MIDDLE(b) !(((b) >> 2) & 1)
#define RIGHT(b) !(((b) >> 1) & 1)
#define RESOLUTION(b) (0x1 << (b))
#define RATE(b) ((b))
