    int putchar(int c); 向标准输出流输出一个字符
    int getchar(void);　从标准输入流读入一个字符
    int putc(int c, FILE *fp);  向任意流fp写一个字符
    int fputc(intc, FILE *fp);　向任意流fp写一个字符
    int getc(FILE *fp);  从任意流读入一个字符
    int fgetc(FILE *fp);　从任意流读入一个字符　
    fputc(int c, stdout) 相当于　putchar(int c)
    getc(stdin) 相当于　getchar();

    #define putchar(c) putc(c,stdout)
    #define getchar(c) getc(c, stdout)
        上面这几个都是通过宏来实现的，　一个是宏速度快，　但是宏是不能给参数设置类型的，　所以就有fputc（） fgetc（）　这种函数的处理方法可供选择



    int puts(const char *s);  将字符串s写人标准输出stdout，并在其后添加一个换行符
    int fputs(const char *s, FILE *fp);  将字符串s写人fp所指向的文件中
    与puts 相同的是：　若出现写入错误，　则返回EOF, 否则返回一个非负数
    与puts不同的是：　它不会自动添加换行符，除非字符串本身含有换行符

    char *gets(char *s);  从标准输入流stdin读取字符串存入到＊ｓ指定的缓存区，　然后返回首地址，　读到换行符时停止
        容易发生缓存区溢出
    char *fgets(char *s, int n, FILE *fp); 从fp所指定的文件中读取字符串，最多读取n-1个字符
    用fgets(buf, sizeof(buf), stdin) 代替　gets(buf) 更安全
    gets 和　fgets 相同点：
        都会在字符串末尾添加'\0'
        读取失败或者到达文件尾部都返回NULL
    gets 和　fgets 不同点：
        对换行符的处理不同
                gets 不保留'\n'替换为'\0'
                fgets 保留并存储'\n'

# Linux 多线程编程
@(操作系统)[线程|clion][pthread|thread]

## 线程相关函数 
### 1. pthread_create创建线程
![Alt text](./QQ截图20160113225709.png)

#### 1.1 线程的属性相关函数
在线程的创建接口`pthread_create`的参数列表中有一个类型为`pthread_attr_t`的参数`attr`，该参数用于指定新建线程的相关属性。一个线程的主要属性包括：`线程栈的地址及大小`，`线程的调度策略`与`优先级`，还有`线程是否处于分离状态`等

- 属性的初始化与销毁:
```cmake
#include <pthread.h>
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destory(pthread_attr_t *attr);
```
`pthread_attr_init`函数必须在`pthread_create`函数之前调用

- 获取指定线程的属性:
```cmake
#define _GNU_SOURCE
#include <pthread.h>
int pthread_getattr_np(pthread_t thread, pthread_attr_t *attr);
```
`注意`:若要使用该接口，在`#include <pthread.h>`前，需先定义宏`_GNU_SOURCE`，因为`pthread_getattr_np`属于GNU扩展接口，而非POSIX标准。

- 设置和获取线程的分离状态:
```cmake
#include <pthread.h>
int pthread_attr_setdetachestate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachestate(pthread_attr_t *attr, int *detachstate);
```

- detachstate参数的取值:
	- PTHREAD_CREATE_JOINABLE，新建线程处于可连接状态，该值为不指定属性创建线程时的默认值。
	- PTHREAD_CREATE_DETACHED，新建线程处于分离状态。

- 设置和获取线程栈地址及大小:
```cmake
#include <pthread.h>
int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);
int pthread_attr_getstack(pthread_attr_t *attr, void **stackaddr, size_t *stacksize);
```

示例代码:
```cmake
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define handle_error_en(en,msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static void display_pthread_attr(pthread_attr_t *attr,  char *prefix)
{
    int s,i;
    size_t v;
    void *stkaddr;
    struct sched_param sp;

    /*获取线程的分离状态*/
    s = pthread_attr_getdetachstate(attr, &i);
    if (s != 0 )
        handle_error_en(s, "pthread_attr_getdetachstate");

    printf("%sDetach state        = %s\n", prefix,
           (i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
           (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
           "???");

    /*获取线程的栈地址及大小*/
    s = pthread_attr_getstack(attr, &stkaddr, &v);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getstack");

    printf("%sStack address       = %p\n", prefix, stkaddr);
    printf("%sStack size          = 0x%x bytes\n", prefix, (unsigned)v);
}

static void *thread_start(void *arg)
{
    int s;
    pthread_attr_t gattr;

    s = pthread_getattr_np(pthread_self(), &gattr);
    if ( s != 0 )
        handle_error_en(s, "pthread_getattr_np");

    printf("Thread attributes:\n");
    display_pthread_attr(&gattr, "\t");

    exit(EXIT_SUCCESS); /*停止所有进程*/
}

int main(int argc, char *argv[])
{
    pthread_t thr;
    pthread_attr_t attr;
    pthread_attr_t *attrp;
    int s;

    attrp = NULL;

    /*若执行该程序带了栈大小参数,则设置新建线程为分离状态, 且根据命令行参数设置新栈地址及大小*/
    if (argc > 1){
        size_t stack_size;
        void *sp;

        attrp = &attr;

        s = pthread_attr_init(&attr);
        if ( s != 0 )
            handle_error_en(s, "pthread_attr_init");

        s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        if ( s != 0 )
            handle_error_en(s, "pthread_attr_setdetachstate");

        /*设置线程地址及大小*/
        stack_size = strtoul(argv[1], NULL, 0);
        /*分配线程栈空间*/
        s = posix_memalign(&sp, sysconf(_SC_PAGESIZE), stack_size);
        if (s != 0)
            handle_error_en(s, "posix_memalign");

        printf("posix_memalign() allocated at %p\n", sp);

        s = pthread_attr_setstack(&attr, sp, stack_size);
        if (s != 0)
            handle_error_en(s, "pthread_attr_setstack");
    }

    s = pthread_create(&thr, attrp, &thread_start, NULL);

    if (s != 0)
        handle_error_en(s, "pthread_create");

    if (attrp != NULL) {
        s = pthread_attr_destroy(attrp);
        if (s != 0)
            handle_error_en(s, "pthread_attr_destroy");
    }

    pause();    /* 当其他线程调用exit()函数的时候，该线程终止 */
}
```
编译命令:
`gcc -o thread_attr_sample thread_attr_sample.c -lpthread`
演示效果：
![Alt text](./1498029876137.png)

### 2. 线程的一次性初始化及特有数据
#### 2.1 一次性初始化
假设有以下需求，在我们的多线程程序中，我们几乎同时创建了N个线程(我们无法知道哪个线程会被先执行)，我们期望这N个线程中谁先运行谁就要去负责去调用一个全局的初始化函数做相关的初始化动作，而该初始化函数仅能被调用一次。

线程的一次性初始化函数正是为了解决上述问题而存在的，函数接口如下：
```cmake
#include <pthread.h>
int pthread_once(pthread_once_t *once_control, void (*init)(void));
```
> 函数说明：该函数用于保证无论有多少个线程调用该函数，也仅会执行一次由init指向的函数实现。

> - 参数说明：
- once_control(输入参数)，指针，指向通过以下方式初始化的全局变量pthread_once_t once_var = PTHREAD_ONCE_INIT;
- init(输入参数)，函数指针，执行仅能被执行一次的初始化函数。

> - 返回值：
- 成功，返回0
- 失败

#### 2.2 特有数据
所谓的线程特有数据，说白了，就是一块看起来是全局的数据概念，但是其实每个线程都有其特有的数据内容，因此每个线程都需要各自不受干扰的内存用来存放数据。线程特有数据的接口如下：
```cmake
#include <pthread.h>
int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));
int pthread_setspecific(pthread_key_t key, const void *value);
void *pthread_getspecific(pthread_key_t key);
```
> 函数说明：
> - pthread_key_create， 创建一个全局唯一的key，用于表示一个数据概念
> - pthread_setspecific， 用于线程给某个数据概念分配内存
> - pthread_getspecific， 用于线程针对某个数据概念获取其对应的内存(每个线程获取的内存是不一样的)，如果函数返回NULL值说明线程还未对该数据概念分配内存


- 接口使用思路如下：
1. 先用pthread_key_create创建一个全局的key，用于表示一块全局的数据概念。
2. 每个线程在使用该数据概念时，先通过pthread_getspecific查询该线程是否为该数据概念分配了内存
3. 如果线程未对该数据概念分配内存，使用pthread_setspecific为该数据概念分配特有内存
4. 如果线程已对该数据概念分配内存，直接操作该内存

- 由于一个数据概念对应一个key，即对于一个数据概念而言不管有多少个线程pthread_key_create仅需要被调用一次，因此pthread_key_create经常在pthread_once函数里被调用。
- pthread_key_create函数中有一个参数destructor，提供了一种释放线程特有数据内存的机制，当某个线程针终止时，如果该线程针对该key分配了内存，那么destructor函数就会被调用，传递给destructor函数的参数就是该线程针对该key分配的内存指针。

下面我们来演示，如何使用线程特有数据来改进我们的mystrerror函数：

```cmake
/* filename: specific_data.c
*  desp: An implementation of mystrerror() that is not thread-safe
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerrorKey;

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define MAX_ERROR_LEN 256

/*线程特有数据的析构函数*/
static void destructor(void *buf)
{
    free(buf);
}


static void createKey(void)
{
    int s;
    /*在phtread_once函数里创建特有数据的key，
    哪个线程先调用就哪个线程创建key*/
    s = pthread_key_create(&strerrorKey, destructor);
    if (s != 0)
        handle_error_en(s, "pthread_key_create");
}

char *mystrerror(int err)
{
    int s;
    char *buf;

    /*一次性初始化函数*/
    s = pthread_once(&once, createKey);
    if (s != 0)
        handle_error_en(s, "pthread_once");

    /*获取线程特有数据*/
    buf = pthread_getspecific(strerrorKey);
    if (buf == NULL) { /*第一次获取为NULL, 线程需要分配内存*/
        buf = malloc(MAX_ERROR_LEN);
        if (buf == NULL)
            handle_error("malloc");
        /*设置特有数据内存*/
        s = pthread_setspecific(strerrorKey, buf);
        if (s != 0)
            handle_error_en(s, "pthread_setspecific");
    }

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
```

编译命令：

`gcc -o mystrerror_test2 main.c mystrerror2.c -lphtread`

### 3. 线程的取消
一个线程可以对另一个线程提出取消申请，即线程被动终止的一种情况。向某个线程提出取消申请的接口：
```cmake
#include <pthread.h>
int pthread_cancel(pthread_t thread);
```

- 如果一个线程因响应pthread_cancel而终止的，那么连接该线程时，将得到PTHREAD_CANCELED返回值。
- 向线程提出取消申请，与线程真正发生终止之间，是异步的。即向线程发出取消申请，被申请线程不一定会马上终止，而是等到某个可以发生终止的时机了才终止。可以发生终止的时机，是指当线程调用某些函数时，会响应取消请求，这些函数也称为线程的取消点。 常见的线程取消点有： 
![Alt text](./1498033605229.png)

示例代码：

```cmake
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
```

编译命令：
`gcc -o thread_cancel_sample thread_cancel_sample.c -lpthread`

## 线程互斥
在`线程的基本概念`一节中我们提过，同一个进程的线程共享进程内的绝大部分资源，当一段访问这些共享资源的代码块，有可能被多个线程执行时，那么这段代码块就称为`临界区`。
当有多个线程并发的在临界区执行时，程序的执行结果会出现不确定性，这种情况称之为`竞态条件`。

看下面的实例：
```cmake
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
```

上述代码，每个线程都对glob进行了10000000次加1操作，glob的初值为0，因此理论上程序的执行结果应该是20000000。我们来看一下实际的执行结果。

编译命令：

`gcc -o thread_sync_sample1 thread_sync_sample1.c -lpthread`

从演示结果可以看出，我们的程序执行结果并非如预期的那样，我们执行5次程序，每次所得到的结果都不一样，而且都小于20000000。

出现上述结果的原因就是因为，`thread_routine`函数的代码块

```cmake
loc = glob;
loc++;
glob = loc;
```
    
就是一块临界区，多个线程并发的访问该临界区，从而出现了`竞态条件`。两个线程有可能以下面的时序执行，从而导致glob的最终值小于20000000:

![Alt text](./1498036483671.png)

多线程编程中，避免出现竞态条件的一项重要解决方案就是，保证多个线程在临界区是互斥的。所谓的互斥，就是指不能同时有多于一个线程进入临界区。

保证临界区互斥的重要技术，就是`互斥锁`。

### 互斥锁的初始化
#### 1.静态初始化：
```cmake
// 静态初始化一个全局的互斥锁
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
```

#### 2.动态初始化：
```cmake
#include <pthread.h>
// 动态分配一个互斥锁
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
// 释放动态分配的互斥锁
int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

#### 3.加锁与解锁:
```cmake
#include <pthread.h>
// 持有互斥锁
int pthread_mutex_lock(pthread_mutex_t *mutex);
// 释放互斥锁
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

使用互斥锁，保证临界区互斥的一般思路是：
1.为该临界区分配一把互斥锁；
2.任何想要进入临界区的线程都必须先持有该互斥锁；
3.持有互斥锁运行于临界区的线程在离开临界区后必须释放该互斥锁；
4.假设某一临界区正在被一个线程A执行着，这意味着线程A持有该临界区的互斥锁M，如果此时有另一个线程B企图持有互斥锁M进入临界区，那么线程B将会进入阻塞状态。

使用互斥锁最常见的错误就是`死锁`，而所谓的死锁是指一个线程为了持有一把互斥锁而永远的阻塞了。

造成死锁原因主要有以下两种：
1. 一个线程试图对其已经持有的互斥锁进行再次加锁；
2. 当有需要持有多把锁时，线程间加锁的顺序不同时，也会造成死锁，如下图所示：
![Alt text](./1498036787931.png)

### 使用互斥锁修正glob的计算问题：
```cmake
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
/*glob为多线程共享资源*/
static int glob = 0;
/* 静态初始化一个全局的互斥锁 */
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


/*子线程访问修改共享资源glob*/
static void *thread_routine(void *arg)
{
  int loc, j;
  for (j = 0; j < 10000000; j++) {
    /*进入临界区前，先持有互斥锁*/
    pthread_mutex_lock(&mtx);
    loc = glob;
    loc++;
    glob = loc;
    /*退出临界区后，释放互斥锁*/
    pthread_mutex_unlock(&mtx);
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
```

## 线程同步
`线程的互斥`一节，我们使用互斥锁解决了多个线程的竞态条件问题。互斥锁的一个主要特点是，谁先拿到锁先就可以优先访问共享资源，因此多个线程访问共享资源的互斥性是得到了保证，但是在某些场合可能还希望确保线程间执行的顺序。如我们有一个共享内存数据资源M，我们整个程序设计需求是要求线程A在M上做了处理之后，线程B才能做处理。这种需要确保多线程间执行先后顺序的技术，称为`线程的同步`。

`条件变量`是线程同步的主要手段。其大致的实现思想就是，线程B，调用条件变量的接口让自身阻塞；线程A，在处理完资源后，通过条件变量接口唤醒正在等待该资源的线程B。

### 条件变量的初始化
#### 1.静态初始化：
```cmake
// 与互斥锁类似静态初始化一个全局的条件变量
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```

#### 2.动态初始化：
```cmake
#include <pthread.h>
int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
```

#### 3.通知和等待条件变量
```cmake
#include <pthread.h>
// 等待一个指定的条件变量
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
// 唤醒一个等待该条件变量的线程
int pthread_cond_signal(pthread_cond_t *cond);
// 唤醒所有等待该条件变量的线程
int pthread_cond_broadcast(pthread_cond_t *cond);
```

需要使用条件变量时，总是意味着有多个线程在使用某一共享资源或状态，而条件变量本身并不提供互斥性的保证，因此，条件变量需要结合互斥锁使用。

我们用伪代码来演示，如何使用条件变量解决上述线程A和线程B的同步问题:

全局初始化
```cmake
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
```
线程A
```cmake
  pthread_mutex_lock(&mtx);
  // 处理资源M
  // 处理完资源M后，设置资源M的状态为线程B可用
  pthread_mutex_unlock(&mtx);

  pthread_cond_signal(&cond);
```  
线程B

```cmake
  pthread_mutex_lock(&mtx);
  while(/*资源M线程B不可用*/)
      pthread_cond_wait(&cond, &mtx);
  // 开始处理资源M
  pthread_mutex_unlock(&mtx);
```
  
线程B的`pthread_cond_wait`函数需要一个互斥锁参数，其内部会执行以下操作:
1. 解锁互斥锁
2. 阻塞调用线程，直到等待的条件变量被唤醒
3. 重新锁定互斥锁

下面我们通过通过来实例演示如何使用条件变量来同步线程。

题目如下：

有两个线程线程1和线程2：线程1的功能就是输出A，线程2的功能就是输出B。

现在有一个文件file初始为空。要让该文件呈如下格式：

file：ABAB....
代码实现：

```cmake
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
```
