#include <minix/drivers.h>
#include "test4.h"

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
			"\t service run %s -args \"test_packet <cnt>\" \n "
			"\t service run %s -args \"test_async <idle_time>\" \n "
			"\t service run %s -args \"test_config <>\" \n "
			"\t service run %s -args \"test_gesture <length> <tolerance>\" \n ", argv[0],
			argv[0], argv[0], argv[0]);

}

static int proc_args(int argc, char *argv[]) {

	unsigned short cnt, igle_time, tolerance;
	short length;
	char *str;
	long num;

	/* check the function to test: if the first characters match, accept it */
	// Teste_packet
	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {
		if (argc != 3) {
			printf("mouse:: wrong no of arguments for test of test_packet() \n");
			return 1;
		}
		if ((cnt = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		printf("mouse:: test_packet(%d)\n", cnt);
		test_packet(cnt);
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
