COMPILER_TYPE= gnu

CC=gcc

PROG=	mouse-maze
SRCS=	utilities.c main.c mouse.c vbe.c video_gr.c timer.c keyboard.c kbc.c mouse-maze.c bitmap.c point.c rectangle.c labyrinth.c MainMenuState.c GameState.c
CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys -llm

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
