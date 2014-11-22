COMPILER_TYPE= gnu

CC=gcc

PROG=	lab5
SRCS=	 lab5.c test5.c vbe.c video_gr.c timer.c keyboard.c kbc.c sprite.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys -llm

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
