/* filename: mystrerror.c
*  desp: An implementation of mystrerror() that is not thread-safe
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define MAX_ERROR_LEN 256

/*全局静态内存块*/
static char buf[MAX_ERROR_LEN];

/*mystrerror返回由错误码errno表示的字符串,*/
char *mystrerror(int err)
{
    if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
        snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
    } else {
        strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
        buf[MAX_ERROR_LEN - 1] = '\0'; /* Ensure null termination */
    }
    return buf;
}

static void *threadFunc(void *arg)
{
    char *str;
    printf("Other thread about to call mystrerror()\n");
    /*子线程获取EPERM错误码的字符信息*/
    str = mystrerror(EPERM);
    printf("Other thread: str (%p) = %s\n", str, str);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t;
    int s;
    char *str;
    /*主线程获取EINVAL错误码的字符信息*/
    str = mystrerror(EINVAL);
    printf("Main thread has called mystrerror()\n");
    s = pthread_create(&t, NULL, threadFunc, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_create");
    s = pthread_join(t, NULL);
    if (s != 0)
        handle_error_en(s, "pthread_join");
    printf("Main thread: str (%p) = %s\n", str, str);

    exit(EXIT_SUCCESS);
}