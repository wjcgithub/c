//
// Created by evolution on 17-6-21.
//

/*
* file name:thread_cancel.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static void *thread_routine(void *arg)
{
    int j;

    printf("New thread started\n"); /*这里有可能是一个线程取消点 */
    for (j = 1; ; j++) {
        printf("Loop %d\n", j); /*这里也可能是一个线程取消点*/
        sleep(1); /*这里是一个线程取消点*/
    }
    /* 线程执行流程无法到达此处 */
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thr;
    int s;
    void *res;
    /*创建新线程*/
    s = pthread_create(&thr, NULL, thread_routine, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_create");

    /*让新线程飞一会儿*/
    sleep(3);

    /*取消新建线程*/
    s = pthread_cancel(thr);
    if (s != 0)
        handle_error_en(s, "pthread_cancel");
    /*等待新线程退出*/
    s = pthread_join(thr, &res);
    if (s != 0)
        handle_error_en(s, "pthread_join");
    /*判断新线程终止是否是响应取消请求而终止的*/
    if (res == PTHREAD_CANCELED)
        printf("Thread was canceled\n");
    else
        printf("Thread was not canceled (should not happen!)\n");

    return 0;
}