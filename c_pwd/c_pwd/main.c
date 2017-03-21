#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

void firstPwd()
{
    char buf[1024];
    char *cwd = getcwd(buf, sizeof(buf));

    if (NULL == cwd) {
        perror("Get current working directory fail. \n");
        exit(-1);
    } else {
        printf("Current working directory is : %s\n", cwd);
    }
}

struct stat file_stat;
void print_file_stat(struct stat *fs)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("inode: \t\t\t\t%ld\n", fs->st_ino);
    printf("protection: \t\t\t%o\n", fs->st_mode);
    printf("number of hard links: \t\t%lu\n", fs->st_nlink);
    printf("user ID of owner: \t\t%d\n", fs->st_uid);
    printf("group ID of owner: \t\t%ld\n", fs->st_gid);
    printf("file size in bytes: \t\t%ld\n", fs->st_size);
    printf("time of last access: \t\t%s", ctime(&fs->st_atime));
    printf("time of last modification: \t\t%s", ctime(&fs->st_mtime));
    printf("time of last change: \t\t%s", ctime(&fs->st_ctime));

}

int main(int argc, char *argv[])
{
//    firstPwd();
    if (2 != argc) {
        fprintf(stderr, "Usage: %s filename....\n", argv[0]);
        exit(-1);
    }

    if (0 != stat(argv[1], &file_stat)) {
        perror("stat");
        exit(-1);
    }

    print_file_stat(&file_stat);

    return 0;
}
