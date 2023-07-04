#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("USAGE: %s $sec\n", argv[0]);
		exit(1);
	}
	int sleep_s = atoi(argv[1]);
	sleep(sleep_s);

	exit(0);
}
