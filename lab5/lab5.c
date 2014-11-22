// Código do lab passado, necessário alterar, tal como no makefile

#include <minix/drivers.h>
#include "video_gr.h"
#include "pixmap.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char** argv) {
	/* Initialize service */
	sef_startup();
	sys_enable_iop(SELF);

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;

}

static void print_usage(char *argv[]) {
	printf(
			"Usage: one of the following:\n"
					"\t service run %s -args \"test_init <mode> <delay>\" \n "
					"\t service run %s -args \"test_square <x> <y> <size> <color>\" \n "
					"\t service run %s -args \"test_line <xi> <yi> <xf> <yf> <color>\" \n "
					"\t service run %s -args \"test_xpm <xi> <yi> <*xpm[]>\" \n "
					"\t service run %s -args \"test_move <xi> <yi> <*xpm[]> <hor> <delta> <time>\" \n "
					"\t service run %s -args \"test_controller <>\" \n ",
			argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);

}

static int proc_args(int argc, char *argv[]) {

	unsigned short mode, delay, xi, yi, xf, yf, size, color, hor, time;
	short length;
	char *xpm;
	short delta;

	/* check the function to test: if the first characters match, accept it */
	// Test_init
	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {
		if (argc != 4) {
			printf("video_graphics:: wrong no of arguments for test_init() \n");
			return 1;
		}
		if ((mode = parse_ulong(argv[2], 16)) == ULONG_MAX)
			return 1;
		if ((delay = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		printf("video_graphics:: test_init(%d, %d)\n", mode, delay);
		test_init(mode, delay);
		return 0;
	}

	// Test_square
	else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0) {
		if (argc != 6) {
			printf(
					"video_graphics:: wrong no of arguments for test_square() \n");
			return 1;
		}
		if ((xi = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((size = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((color = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		printf("video_graphics:: test_square(%d,%d,%d,%d)\n", xi, yi, size,
				color);
		test_square(xi, yi, size, color);
		return 0;
	}
	// Test_line
	else if (strncmp(argv[1], "test_line", strlen("test_line")) == 0) {
		if (argc != 7) {
			printf("video_graphics:: wrong no of arguments for test_line() \n");
			return 1;
		}
		if ((xi = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((xf = parse_ulong(argv[4], 10)) == ULONG_MAX)
			return 1;
		if ((yf = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		if ((color = parse_ulong(argv[6], 10)) == ULONG_MAX)
			return 1;
		printf("video_graphics:: test_line(%d,%d,%d,%d)\n", xi, yi, xf, yf,
				color);
		test_line(xi, yi, xf, yf, color);
		return 0;
	}
	// Test_xpm
	if (strncmp(argv[1], "test_xpm", strlen("test_xpm")) == 0) {
		if (argc != 5) {
			printf(
					"video_graphics:: wrong no of arguments for test of test_xpm() \n");
			return 1;
		}
		if ((xi = parse_long(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		xpm = argv[4];
		char **xpm_array;
		if (strncmp(xpm, "pic1", strlen("pic1")) == 0)
			xpm_array = pic1;
		else if (strncmp(xpm, "pic2", strlen("pic2")) == 0)
			xpm_array = pic2;
		else if (strncmp(xpm, "pic3", strlen("pic3")) == 0)
			xpm_array = pic3;
		else if (strncmp(xpm, "cross", strlen("cross")) == 0)
			xpm_array = cross;
		else if (strncmp(xpm, "penguin", strlen("penguin")) == 0)
			xpm_array = penguin;
		else {
			printf("ERROR!!!\n");
			return 1;
		}
		printf("video_graphics::test_xpm(%d, %d, %s) \n", xi, yi, xpm);
		test_xpm(xi, yi, xpm_array);
		return 0;
	}
	// Test_move
	if (strncmp(argv[1], "test_move", strlen("test_move")) == 0) {
		if (argc != 8) {
			printf(
					"video_graphics:: wrong no of arguments for test of test_move() \n");
			return 1;
		}
		if ((xi = parse_long(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;
		if ((hor = parse_ulong(argv[5], 10)) == ULONG_MAX)
			return 1;
		if ((delta = parse_ulong(argv[6], 10)) == ULONG_MAX)
			return 1;
		if ((time = parse_ulong(argv[7], 10)) == ULONG_MAX)
			return 1;
		xpm = argv[4];
		char **xpm_array;
		if (strncmp(xpm, "pic1", strlen("pic1")) == 0)
			xpm_array = pic1;
		else if (strncmp(xpm, "pic2", strlen("pic2")) == 0)
			xpm_array = pic2;
		else if (strncmp(xpm, "pic3", strlen("pic3")) == 0)
			xpm_array = pic3;
		else if (strncmp(xpm, "cross", strlen("cross")) == 0)
			xpm_array = cross;
		else if (strncmp(xpm, "penguin", strlen("penguin")) == 0)
			xpm_array = penguin;
		else {
			printf("ERROR!!!\n");
			return 1;
		}
		printf("video_graphics::test_xpm(%d, %d, %s) \n", xi, yi, xpm);
		test_move(xi, yi, xpm_array, hor, delta, time);
		return 0;
	} else {
		printf("video_graphics:: non valid function \"%s\" to test\n", argv[1]);
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
		printf("lab5: parse_ulong: no digits were found in %s \n", str);
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
		printf("lab5: parse_long: no digits were found in %s \n", str);
		return LONG_MAX;
	}

	/* Successful conversion */
	return val;
}
