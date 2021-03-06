#include <minix/drivers.h>
#include "test3.h"

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
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"test_scan <ass>\" \n "
			"\t service run %s -args \"test_leds <n> <*leds>\" \n "
			"\t service run %s -args \"test_timed_scan <n>\" \n ", argv[0],
			argv[0], argv[0], argv[0]);

}

static int proc_args(int argc, char *argv[]) {

	unsigned short ass, n, *leds;
	char *str;
	long num;

	/* check the function to test: if the first characters match, accept it */
	// Test_Scan
	if (strncmp(argv[1], "test_scan", strlen("test_scan")) == 0) {
		if (argc != 3) {
			printf("kbd: wrong no of arguments for test of kbd_test_scan() \n");
			return 1;
		}
		if ((ass = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		printf("kbd:: kbd_test_scan(%d)\n", ass);
		kbd_test_scan(ass);
		return 0;
	}

	//Test_leds
	else if (strncmp(argv[1], "test_leds", strlen("test_leds")) == 0) {
		if (argc < 3) {
			printf(
					"kbd:: wrong no of arguments for test of kbd_test_leds() \n");
			return 1;
		}
		printf("kbd:: kbd_test_leds()\n");
		unsigned short *l;
		l = malloc((argc - 2) * sizeof(unsigned short));
		int i=0;
		for (i; i<argc - 2; i++)
		{
			l[i]=parse_ulong(argv[2+i], 10);
			if (l[i] > 2 || l[i] < 0)
			{
				printf("\nError: 0 <= led's bit <= 2\n\n");
				return 1;
			}
		}
		kbd_test_leds(argc - 2, l);
		return 0;
	}
	// test_timed_scan
	else if (strncmp(argv[1], "test_timed_scan", strlen("test_timed_scan"))
			== 0) {
		if (argc != 3) {
			printf(
					"kbd:: wrong no of arguments for test of kbd_test_timed_scan() \n");
			return 1;
		}
		if ((n = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		printf("kbd:: timer_test_int(%d)\n", n);
		kbd_test_timed_scan(n);
		return 0;
	} else {
		printf("kbd:: non valid function \"%s\" to test\n", argv[1]);
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
		printf("lab3: parse_ulong: no digits were found in %s \n", str);
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
		printf("lab3: parse_long: no digits were found in %s \n", str);
		return LONG_MAX;
	}

	/* Successful conversion */
	return val;
}
