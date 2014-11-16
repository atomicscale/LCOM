// Código do lab passado, necessário alterar, tal como no makefile

#include <minix/drivers.h>
#include "video_gr.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char** argv) {
	/* Initialize service */
	sef_startup();
	//sys_enable_iop(SELF);

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;

}

static void print_usage(char *argv[]) {
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"test_init <mode> <delay>\" \n "
			"\t service run %s -args \"test_square <x> <y> <size> <color>\" \n "
			"\t service run %s -args \"test_line <xi> <yi> <xf> <yf> <color>\" \n "
			"\t service run %s -args \"test_xpm <xi> <yi> <*xpm[]>\" \n "
			"\t service run %s -args \"test_move <xi> <yi> <*xpm[]> <hor> <delta> <time>\" \n "
			"\t service run %s -args \"test_controller <>\" \n ", argv[0],
			argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0],
			argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0],
			argv[0], argv[0], argv[0]);

}

static int proc_args(int argc, char *argv[]) {

	unsigned short mode, delay, igle_time, tolerance;
	short length;
	char *str;
	long num;

	/* check the function to test: if the first characters match, accept it */
	// Test_init
	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {
		if (argc != 4) {
			printf("video_graphics:: wrong no of arguments for test of test_init() \n");
			return 1;
		}
		if ((mode = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((delay = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		printf("video_graphics:: test_init(%d, %d)\n", mode, delay);
		test_init(mode, delay);
		return 0;
	}

	// Test_async
	else if (strncmp(argv[1], "test_async", strlen("test_async")) == 0) {
		if (argc != 3) {
			printf("mouse:: wrong no of arguments for test of test_async() \n");
			return 1;
		}
		if ((igle_time = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		printf("mouse:: test_async(%d)\n", igle_time);
		test_async(igle_time);
		return 0;
	}
	// Test_config
	else if (strncmp(argv[1], "test_config", strlen("test_config")) == 0) {
		if (argc != 2) {
			printf(
					"mouse:: wrong no of arguments for test of test_config() \n");
			return 1;
		}
		printf("mouse:: test_config() \n");
		test_config();
		return 0;
	}
	// Test_gesture
	else if (strncmp(argv[1], "test_gesture", strlen("test_gesture")) == 0) {
		if (argc != 4) {
			printf("mouse:: wrong no of arguments for test of test_gesture() \n");
			return 1;
		}
		if ((length = parse_long(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((tolerance = parse_ulong(argv[3],10)) == ULONG_MAX)
			return 1;
		printf("mouse:: test_gesture(%d, %d) \n", length, tolerance);
		test_gesture(length, tolerance);
		return 0;
	} else {
		printf("mouse:: non valid function \"%s\" to test\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtoul(str, &endptr, base);

	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtol");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("lab4: parse_ulong: no digits were found in %s \n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static long parse_long(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtol(str, &endptr, base);

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MAX;
	}

	if (endptr == str) {
		printf("lab4: parse_long: no digits were found in %s \n", str);
		return LONG_MAX;
	}

	/* Successful conversion */
	return val;
}
