//
// Created by evolution on 17-6-21.
//
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
/*glob为多线程共享资源*/
static int glob = 0;

/*子线程访问修改共享资源glob*/
static void *thread_routine(void *arg)
{
    int loc, j;
    for (j = 0; j < 10000000; j++) {
        loc = glob;
        loc++;
        glob = loc;
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2;
    int s;

    /*创建两个线程并发访问修改共享资源glob*/
    s = pthread_create(&t1, NULL, thread_routine, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_create");
    s = pthread_create(&t2, NULL, thread_routine, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_create");

    /*等待子线程退出*/
    s = pthread_join(t1, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_join");
    s = pthread_join(t2, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_join");
    /*输出结果*/
    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}