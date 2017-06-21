/*
 * main.c
 *
 *  Created on: 2017年6月20日
 *      Author: evolution
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *thread_routine1(void *arg)
{
    fprintf(stdout, "Thread1: hello world!\n");
    sleep(1);
    return NULL;
}

void *thread_routine2(void *arg)
{
    fprintf(stdout, "thread2: I'm running...\n");
    pthread_t main_thread = (pthread_t)arg;

    /*分离自我, 不能在被链接*/
    pthread_detach(pthread_self());

    /*
     * 判断主线程ID与子线程2ID是否相等
     */
    if (!pthread_equal(main_thread, pthread_self())){
        fprintf(stdout, "thread2: main thread id is not equal thread2\n");
    }

    /*等待主线程终止*/
    pthread_join(main_thread, NULL);
    fprintf(stdout, "thread2: main thread exit!\n");

    fprintf(stdout, "thread2: exit!\n");
    fprintf(stdout, "thread2: process exit!\n");
    /*子线程2在此终止，进程退出*/
    pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
    /*创建子线程1*/
    pthread_t t1;
    if (pthread_create(&t1, NULL, thread_routine1, NULL) !=0 ) {
        fprintf(stderr, "create thread fail.\n");
        exit(-1);
    }

    /*等待子线程1终止*/
    pthread_join(t1, NULL);
    fprintf(stdout, "main thread: thread1 terminated!\n\n");

    /*创建子线程2，并将主线程ID传递给子线程2*/
    pthread_t t2;

    if (pthread_create(&t2, NULL, thread_routine2, (void *)pthread_self())!=0) {
        fprintf(stderr, "create thread fail.\n");
        exit(-1);
    }

    fprintf(stdout, "main thread: exit!\n");
    pthread_exit(NULL);

    fprintf(stdout, "main thread: never reach here!\n");

    return 0;
}
