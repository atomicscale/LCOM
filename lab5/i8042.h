#pragma once

#include <minix/sysutil.h>
#include <minix/syslib.h>

#define BIT(n) (0x01<<(n))

#define OBF BIT(0)
#define IBF BIT(1)
#define TO_ERR BIT(6)
#define PAR_ERR BIT(7)

#define IRQ_KBD 1

#define STAT_REG 0x64
#define IN_BUF 0x60
#define OUT_BUF 0x60
#define KBC_IO_MAX_TRIES 3

#define SW_LEDS 0xED

#define RESEND 0xFE
#define ERROR 0xFC
#define ACK 0xFA


#define DELAY_US    20000
