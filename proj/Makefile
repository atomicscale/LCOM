COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	 proj.c mouse.c vbe.c video_gr.c timer.c keyboard.c kbc.c sprite.c test.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys -llm

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
