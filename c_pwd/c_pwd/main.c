#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define MAX_DIR_DEPTH 256

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

void dir_content_list(char *dir)
{
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    struct stat st;
    char *newdir;
    if (NULL == (dp = opendir(dir))) {
        fprintf(stderr, "Can not open Input Directory [%s]\n", dir);
        exit(-1);
    } else {
        printf("Directory [%s] Content List: \n", dir);
//         stat(dir,&st);
//        if (S_ISDIR(st.st_mode)) {
//            strcpy(newdir, dir);
//            strcat(newdir, "/");
//            dptr = readdir(dp);
//            strcat(newdir, dptr->d_name);
//            printf("The new dir name is %s", newdir);
//            dir_content_list(newdir);
//        }
        while (NULL != (dptr = readdir(dp))) {
            printf("inode-number: %-10ld \t filename: %s\n", dptr->d_ino, dptr->d_name);
        }
    }

    closedir(dp);
}

//获取文件的inode号
ino_t get_ino_byname(char *filename)
{
    struct stat file_stat;
    if (0 != stat(filename, &file_stat)) {
        perror("stat");
        exit(-1);
    }

    return file_stat.st_ino;
}

//根据inode-number在当前目录中查找对应的文件名
char *find_name_byino(ino_t ino)
{
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    char *filename = NULL;
    if (NULL == (dp = opendir("."))) {
        fprintf("stderr", "Can not open Current Directory\n");
        exit(-1);
    } else {
        while (NULL != (dptr = readdir(dp))) {
            if (dptr->d_ino == ino) {
                filename = strdup(dptr->d_name);
                break;
            }
        }
        closedir(dp);
    }

    return filename;
}

int main(int argc, char *argv[])
{
//    firstPwd();


// 打印文件信息
//    if (2 != argc) {
//        fprintf(stderr, "Usage: %s filename....\n", argv[0]);
//        exit(-1);
//    }
//    if (0 != stat(argv[1], &file_stat)) {
//        perror("stat");
//        exit(-1);
//    }
//    print_file_stat(&file_stat);


//　打印目录列表内容
//    if (2 != argc) {
//        fprintf(stderr, "Usage: %s directory...\n", argv[0]);
//        exit(-1);
//    }
//
//    dir_content_list(argv[1]);


// 自定义pwd
    char *dir_stack[MAX_DIR_DEPTH];
    unsigned current_depth = 0;
    for (;;) {
        //获取当前目录的ino
        ino_t current_ino = get_ino_byname(".");

        //获取当前目录父目录的ino
        ino_t parent_ino = get_ino_byname("..");

        //判断当前ino和父目录ino是否一致，如果一致则推出for循环
        if (current_ino == parent_ino) {
            break;
        }

        //如果不一至，继续向父目录查询
        chdir("..");
        dir_stack[current_depth++] = find_name_byino(current_ino);

        //如果目录达到指定的深度，则报告一个错误
        if (current_depth >= MAX_DIR_DEPTH) {
            fprintf(stderr, "Directory trrr is too deep. \n");
            exit(-1);
        }
    }

    //输出完整路径名
    int i = current_depth - 1;
    for (i = current_depth -1; i>=0; i--) {
        fprintf(stdout, "/%s", dir_stack[i]);
    }

    fprintf("stdout", "%s\n", current_depth==0?"/":"");

    return 0;
}
