#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>


pthread_t threads[2];
char writer_char[2] = {'A', 'B'};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


struct file_res{
    pthread_t *writer; /*当前文件可以被哪个线程写入*/
    int fd; /*文件描述符*/
}file_res =
        {
                .writer=&threads[0],/*file初始化可以被线程1写入'A'*/
        };

/*线程1和线程2的入口函数*/
void *writer_routine(void *arg)
{
    int index = (intptr_t)arg;
    int i = 0;
    int next_index=0;
    printf("thread %d is running, and will write '%c' to file\n", index, writer_char[index]);

    while(1)
    {
        if (0!=pthread_mutex_lock(&mutex))
            exit(-1);
        for(;;) {

            /*如果当前线程可写file, 执行写操作*/
            if (&threads[index]==file_res.writer) {
                write(file_res.fd, &writer_char[index],
                      sizeof(writer_char[index]));

                /*更新下一个可写线程*/
                next_index = (index+1)%2;
                file_res.writer = &threads[next_index];

                /*执行写操作后，break for循环通过条件变量通知其他线程写*/
                break;
            }

            /*当前线程不可写，等待其他线程唤醒*/
            pthread_cond_wait(&cond,&mutex);
        }

        if (0!=pthread_mutex_unlock(&mutex))
            exit(-1);

        /*唤醒下一个线程*/
        pthread_cond_signal(&cond);

    }
}

int main(int argc, char* argv[])
{

    /*创建空文件file*/
    char file_name[] = "file";
    if ((file_res.fd = open(file_name, O_RDWR|O_CREAT|O_TRUNC, 0666)) < 0)
    {
        printf("open %s error.\n", file_name);
        exit(-1);
    }

    /*创建线程1和线程2*/
    int i;
    for (i=0; i<(sizeof(threads)/sizeof(pthread_t)); i++)
    {
        if(pthread_create(&threads[i], NULL, writer_routine, (void *)(intptr_t)i))
        {
            printf("create writer thread error\n");
            exit(-1);
        }
    }

    /*主线程退出*/
    pthread_exit(NULL);
}