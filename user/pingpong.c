#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main (int argc, char **argv) {
	int fd0[2];
	int fd1[2];

	if (pipe(fd0) < 0) {
		exit(1);
	}
	if (pipe(fd1) < 0) {
		exit(1);
	}

	if (fork() == 0) {
		// child process
		int a;
		close(fd0[0]);
		close(fd1[1]);

		read(fd1[0], &a, 1);
		printf("%l: received ping\n", getpid());
		write(fd0[1], "b", 1);
		exit(0);
	} else {
		// parent process
		int a;
		close(fd0[1]);
		close(fd1[0]);

		write(fd1[1], "a", 1);
		read(fd0[0], &a, 1);
		printf("%l: received pong\n", getpid());
		exit(0);
	}
	
	
}
