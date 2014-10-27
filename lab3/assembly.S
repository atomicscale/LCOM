#define OUT_BUF 0x60

.global _handler_asm

.text
_handler_asm:
	xor %eax, %eax
	inb $OUT_BUF, %al
	ret
