#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define MAX_INPUT_LEN 100

int main (int argc, char **argv) {
    int i = 0;
    int rc;
    char *buf = (void*)0;
    char **new_argv= (void*)0;

    if (argc > MAXARG) {
        fprintf(2, "too much argv\n");
        exit(1);
    }
    
    new_argv = malloc(sizeof(void*)*10);
    buf = malloc(MAX_INPUT_LEN);
    
    if (!buf || !new_argv) {
        fprintf(2, "xargs: malloc failed\n");
        exit(1);
    }
    
    // append stdin to argv
    for (i = 0; i < argc; ++i) {
        new_argv[i] = argv[i];
    }
    i = 0;
    while (1) {
        if (i >= MAX_INPUT_LEN) {
            fprintf(2, "xargs: too long input\n");
            exit(1);
        }
        rc = read(0, buf+i, 1);

        i++;
        if (buf[i-1] == ' ' || buf[i-1] == '\n') {
            buf[i-1] = '\0';
            new_argv[argc]= buf;
            buf += (i);
            i = 0;
            argc++;
        }
        if (rc <= 0) {
            break;
        }
    }
    new_argv[argc] = (void*)0;

    if (fork() == 0) {
        // child process
        exec(new_argv[1], new_argv+1);
    } else {
        // parent process
        wait(0);
    }

    exit(0);
}
