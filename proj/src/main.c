#include <minix/drivers.h>
#include "mouse-maze.h"

int main(int argc, char** argv) {
	sef_startup();
	sys_enable_iop(SELF);
	test_pointer();
	return 0;
}
