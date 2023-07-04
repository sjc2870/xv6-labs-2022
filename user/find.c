#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define NULL ((void*)0)

void strcat(char *str, const char *fix) {
    int i = 0;
    int j = strlen(str);

    for (i = 0; fix[i]; ++i, ++j) {
        str[j] = fix[i];
    }

    str[j] = '\0';

}

#define LEN 100   // can not be too large, otherwise page fault

void find(char *dir, char *name) {
    struct dirent de;
    struct stat st;
    int fd;
    char cur_dir[LEN] = {0};

    if ((fd = open(dir, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    if (fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }
    
    memcpy(cur_dir, dir, strlen(dir));
    strcat(cur_dir, "/");
    if (st.type == T_DIR) {
        int ret;
        if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof cur_dir){
            printf("find: path too long\n");
            return;
        }
        char path[LEN] = {0};
        while ((ret = read(fd, &de, sizeof(de))) == sizeof(de)) {
            if (!strlen(de.name)) {
                continue;
            }
            strcpy(path, cur_dir);
            strcat(path, de.name);
            if (stat(path, &st) < 0) {
                printf("find: cannot stat %s\n", path);
                continue;
            }
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                continue;
            }
            if (strcmp(de.name, name) == 0) {
                printf("%s\n", path);
            }

            if (st.type == T_DIR) {
                char next_dir[LEN] = {0};

                strcpy(next_dir, cur_dir);
                strcat(next_dir, de.name);
                find(next_dir, name);
            }
        }

    } else {
        exit(1);
    }
}

int main(int argc, char **argv) {
	char *dir = NULL;
	char *name = NULL;
	
	if (argc != 3) {
		printf("USAGE: %s $dir $name", argv[0]);
        exit(1);
	}
	
    dir = argv[1];
    name = argv[2];

    find(dir,name);

    exit(0);
}
