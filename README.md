# Philosophers

In computer science, the dining philosophers problem is an example problem often used in **concurrent algorithm design** to illustrate **synchronization** issues and techniques for resolving them.

There are N philosphers sitting around a circular table eating spaghetti and discussing philosphy. The problem is that each philosopher needs 2 forks to eat, and there are only N forks, one between each 2 philosophers. Design an algorithm that the philosophers can follow that insures that none starves as long as each philosopher eventually stops eating, and such that the maximum number of philosophers can eat at once.

## Table of contents

- [Philosophers](#philosophers)
  - [Table of contents](#table-of-contents)
  - [Overview](#overview)
  - [Mandatory overview](#mandatory-overview)
  - [Bonus overview](#bonus-overview)
  - [Global rules](#global-rules)
  - [Allowed functions](#allowed-functions)
    - [`memset`](#memset)
    - [`usleep`](#usleep)
    - [`gettimeofday`](#gettimeofday)
    - [`pthread_create`](#pthread_create)
    - [`pthread_detach`](#pthread_detach)
    - [`pthread_join`](#pthread_join)
    - [`pthread_mutex_init`](#pthread_mutex_init)
    - [`pthread_mutex_destroy`](#pthread_mutex_destroy)
    - [`pthread_mutex_lock`](#pthread_mutex_lock)
    - [`pthread_mutex_unlock`](#pthread_mutex_unlock)
    - [`fork` (bonus)](#fork-bonus)
    - [`kill` (bonus)](#kill-bonus)
    - [`waitpid` (bonus)](#waitpid-bonus)
    - [`sem_open` (bonus)](#sem_open-bonus)
    - [`sem_close` (bonus)](#sem_close-bonus)
    - [`sem_post` (bonus)](#sem_post-bonus)
    - [`sem_unlink` (bonus)](#sem_unlink-bonus)
  - [Learning notes](#learning-notes)
    - [Short introduction to threads (pthreads)](#short-introduction-to-threads-pthreads)
    - [Difference between processes and threads](#difference-between-processes-and-threads)
    - [Race Conditions](#race-conditions)
    - [Mutex in C](#mutex-in-c)
    - [Create threads in a loop](#create-threads-in-a-loop)
    - [Get return value from a thread](#get-return-value-from-a-thread)
    - [Pass arguments to threads in C](#pass-arguments-to-threads-in-c)
    - [What are detached threads](#what-are-detached-threads)
    - [Deadlock](#deadlock)

## Overview

1. One or more philosophers sit at a round table. There's a large bowl of spaghetti in the middle of the table.
2. The philosophers alternatively **eat**, **think**, or **sleep**.
3. There are **as many forks as philosophers**.
4. When a philosopher is eating, he/she eats with two forks. One from its left, one from its right.
5. After a philosopher stop eating, they put forks back on the table and start sleeping. Once awake, they start thinking again. The simulation stops when a philosopher dies of starvation.
6. Every philosopher needs to eat and should never starve.
7. Philosophers don't speak with each other.
8. Philosophers don't know if another philosopher is about to die.

Philosopher's routine: **EAT, SLEEP, THINK, REPEAT**.

## Mandatory overview

Mandatory part is about **threads and mutexes**.

Each philosopher should be a thread.

There is one fork between each pair of philosophers. Therefore, if there are several philosophers, each philosopher has a fork on their left side and a fork on their right side. If there is only one philosopher, there should be only one fork on the table.

To prevent philosophers from duplicating forks, you should protect the forks state with a mutex for each of them.

## Bonus overview

Bonus part is about **processes and semaphores**.

All the forks are put in the middle of the table.

They have no states in memory but the number of available forks is represented by a semaphore.

Each philosopher should be a process. But the main process should not be a philosopher.

## Global rules

1. No global variables.
2. Program should take the following arguments:
   1. Number of philosophers
   2. Time to die (ms)
   3. Time to eat (ms)
   4. Time to sleep (ms)
   5. Number of times each philosopher must eat (optional)
3. Each philosopher should have a ID ranging from 1 to `Number of philosophers`
4. Any state change of a philosopher must be logged in such format:
   1. `timestamp_in_ms` *X* has taken a fork
   2. `timestamp_in_ms` *X* is eating
   3. `timestamp_in_ms` *X* is sleeping
   4. `timestamp_in_ms` *X* is thinking
   5. `timestamp_in_ms` *X* died

   > *X* is philosopher ID
5. A message announcing a philosopher died should be displayed no more than **10 ms** after the actual death of the philosopher.
6. The program must not have any [**data races**](https://riptutorial.com/c/example/2622/data-race)

## Allowed functions

Here are some of the allowed functions that I will not do any further research on:

- `printf`
- `malloc`
- `free`
- `write`

### `memset`

```c
#include <stdio.h>
#include <string.h>

void    *memset(void *ptr, int value, size_t num);
```

Sets the first **num** bytes of the block of memory pointed by **ptr** to the specified **value**.

Saw a lot of people used `memset` to initialize a struct in this project. I don't really understand why they did that, I personally think that initialize struct's member with value is so much cleaner. Some article also mentioned that initialize struct's member with value is less expensive than using `memset`.

In some cases where you struct looks like the one below, then I think using memset is reasonable.

```c
struct alphabet {
    char a;
    char b;
}
// or
struct numbers {
    int one;
    int two;
}
```

Something worth your time to read:

- [Stop memsetting structures](https://www.anmolsarma.in/post/stop-struct-memset/)
- Extra resource, because the article above mentions something about structure padding: [Structure Padding](https://www.javatpoint.com/structure-padding-in-c)

### `usleep`

```c
#include <unistd.h>

int usleep(useconds_t usec);

// useconds_t is an unsigned integer. If effective, it returns 0; otherwise, -1.
```

Suspends execution of the calling thread for (at least) `usec` microseconds. The sleep may be lengthened slightly by any system activity or by the time spent processing the call or by the granularity of system timers.

### `gettimeofday`

```c
#include <sys/time.h>

int gettimeofday(struct timeval *restrict tv, struct timezone *restrict tz);
```

The `tv` argument is a `struct timeval` (as specified in `<sys/time.h>`):

```c
struct timeval {
    time_t      tv_sec;     /* seconds */
    suseconds_t tv_usec;    /* microseconds */
}
```

The `tz` argument is a `struct timezone`:

```c
struct timezone {
    int tz_minuteswest; /* minutes west of Greenwich */
    int tz_dsttime;     /* type of DST correction */
}
```

If either `tv` or `tz` is *NULL*, the corresponding structure is not set or returned. Compilation warnings will result if `tv` is NULL.

`gettimeofday` return `0` for success. On error, `-1` is returned and errno is set to indicate the error.

### `pthread_create`

```c
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
```

`pthread_create` function starts a new thread in the calling process. The new thread starts execution by invoking `start_rountine()`; `arg` is passed as the *sole argument* of `start_routine()`.

The new thread terminates in one of the following ways:

1. It calls `pthread_exit`, specifying an exit status value that is available to another thread in the same process that calls `pthread_join`.
2. It returns from `start_routine()`. This is equivalent to calling `pthread_exit` with the value supplied in the return statement.
3. It is canceled (check `pthread_cancel`).
4. Any of the threads in the process calls `exit`, or the main thread performs a return from `main()`. This causes the termination of all threads in the process.

The `pthread_create` function is used to create a new thread, with attributes specified by `attr`, within a process. If `attr` is **NULL**, the default attributes are used. If the attributes specified by `attr` are modified later, the thread's attributes are not affected. Upon successful completion, `pthread_create` stores the ID of the created thread in the location referenced by `thread`.

The `attr` argument points to a `pthread_attr_t` structure whose contents are used at thread creation time to determine attributes for the new thread. It's initialized using `pthread_attr_init`. **But since this subject does not allow us to use this function, just pass in NULL.** If `attr` is NULL, the thread is created with default attributes.

Before returning (successful), `pthread_create()` **stores the ID of the new thread in the buffer pointed to by `thread`**; this identifier is used to refer to the thread in subsequent calls to other pthreads functions.

On success, `pthread_create` returns `0`; on error, it returns an error number, and the contents of `thread` are undefined. When calling this function, needs to catch the return value.

### `pthread_detach`

```c
#include <pthread.h>

int pthread_detach(pthread_t thread);
```

`pthread_detach` function marks the thread identified by `thread` as detached. **When a detached thread terminates, its resources are automatically released back to the system without the need for another thread to join with the terminated thread**.

*Attempting to detach an already detached thread results in unspecified behaviour.*

On success, `pthread_detach` returns `0`; on error, it returns and error number.

### `pthread_join`

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

The `pthread_join` function waits for the thread specified by `thread` to terminate. If that thread has already terminated, then `pthread_join` returns immediately. The thread specified by `thread` must be joinable. A thread object is said to be joinable if it identifies/represent an active thread of execution.

A thread is not joinable if:

    - It was default-constructed
    - If either of its member join or detach has been called
    - It has been moved somewhere

If multiple threads simultaneously try to join with the same thread, the results are undefined. If the thread calling `pthread_join` is canceled, then the target thread will remain joinable, which means it will not be detached.

On success, `pthread_join` returns `0`; on error, it returns an error number.

### `pthread_mutex_init`

```c
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
```

`pthread_mutex_init` initializes the mutex referenced by `mutex` with attributes specified by `attr`. If `attr` is NULL, the default mutex attributes are used. Upon successful initialization, the state of the mutex becomes initialized and unlocked.

*Attempting to initialize an already initialized mutex results in undefined behaviour.*

If successful, the `pthread_mutex_init` functions return `0`. Otherwise, and error number is returned to indicate the error.

### `pthread_mutex_destroy`

```c
#include <pthread.h>

int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

`pthread_mutex_destory` function destorys the mutex object referenced by `mutex`; the mutex object becomes, in effect, uninitialized. An implementation may cause `pthread_mutex_destroy` to set the object referenced by `mutex` to and invalid value. A destroyed mutex object can be re-initialized using `pthread_mutex_init`; the results of otherwise referencing the object after it has been destroyed are undefined.

**It is safe to destroy an initialized mutex that is UNLOCKED.** Attempting to destroy a locked mutex results in undefined behaviour.

If successful, the `pthread_mutex_destroy` functions return `0`. Otherwise, and error number is returned to indicate the error.

### `pthread_mutex_lock`

```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
```

The mutex object referenced by mutex is locked by calling `pthread_mutex_lock()`. **If the mutex is already locked, the calling thread blocks until the mutex becomes available. (wait till the current owner of the same thread unlocked it)** This operation returns with the mutex object referenced by mutex in the locked state with the calling thread as its owner.

If successful, the `pthread_mutex_lock()` functions return zero. Otherwise, an error number is returned to indicate the error.

### `pthread_mutex_unlock`

```c
#include <pthread.h>

int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

The `pthread_mutex_unlock` releases the mutex object referenced by mutex. The manner in which a mutex is released is dependent upon the mutex's type attribute. If there are threads blocked on the mutex object referenced by mutex when pthread_mutex_unlock() is called, resulting in the mutex becoming available.

If successful, the `pthread_mutex_unlock()` functions return zero. Otherwise, an error number is returned to indicate the error.

### `fork` (bonus)

### `kill` (bonus)

### `waitpid` (bonus)

### `sem_open` (bonus)

### `sem_close` (bonus)

### `sem_post` (bonus)

### `sem_unlink` (bonus)

## Learning notes

Resource: [Unix Threads in C](https://youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)

### Short introduction to threads (pthreads)

When using anything from `<pthread.h>`, compile and link your source code with `-pthread`. Like this:

```shell
gcc -g -pthread main.c -o main
```

The basis of creating a thread is that when you create a thread, it executes a function, whatever function you make. In the following section, I will refer that function as a *routine*. Once we have the routine for the thread, we need to define a variable where basically the API can store some information about the thread. The variable type should be `pthread_t`.

To create a thread, we just need to call [`pthread_create`](#pthread_create). Similar to how we do with processes, we need to wait for the process to finish using `wait` or `waitpid`. For thread, we also need to wait for it to finish its execution. We can wait for the thread using [`pthread_join()`](#pthread_join), it's basically the `wait` for thread.

The example below proves that threads are running in parallel. It's always better to check if the `pthread_create` or `pthread_join` will return error value or not. The return value below does not hold any meaning. Suggest to not handle the error like this.

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void    *routine()
{
    printf("Hello from thread\n");
    sleep(3);
    printf("Bye thread\n");
}

int main(void)
{
    pthread_t   t1, t2;

    if (pthread_create(&t1, NULL, &routine, NULL) != 0)
        return (1);
    if (pthread_create(&t2, NULL, &routine, NULL) != 0)
        return (2);
    if (pthread_join(t1, NULL))
        return (3);
    if (pthread_join(t2, NULL))
        return (4);
    return (0);
}
```

### Difference between processes and threads

2 Processes:

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
    int x = 2;
    int pid = fork();
    if (pid == -1)
        return (1);
    if (pid == 0)
    {
        // child process
        x++;
    }
    sleep(2);
    printf("value of x: %d\n", x);
    printf("Hello from process %d\n", getpid());
    if (pid != 0)
        wait(NULL);
    return (0);
}
```

2 Threads:

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int x = 2;

void    *addition_routine()
{
    x++;
    sleep(2);
    printf("PID: %d\n", getpid());
    printf("Value of x: %d\n", x);
}

void    *routine(void)
{
    sleep(2);
    printf("PID: %d\n", getpid());
    printf("Value of x: %d\n", x);
}

int main(void)
{
    pthread_t   t1, t2;

    if (pthread_create(&t1, NULL, &routine, NULL) != 0)
        return (1);
    if (pthread_create(&t2, NULL, &addition_routine, NULL) != 0)
        return (2);
    if (pthread_join(t1, NULL))
        return (3);
    if (pthread_join(t2, NULL))
        return (4);
    return (0);
}
```

The first thing that is noticeable after we run the codes above is that the process ID of the two processes are different, while the process ID of two threads are the same.

Turns out that a single process can contain multiple threads inside. For thread, you **cant** have multiple processes inside a single thread.

The second difference is regarding the memory space used. In the context of process, when we are forking, we are actually duplicating all the variables of parent process into the child process. So we get two different variables and can modify them individually. But for thread, it's another story. All the variables will be in the same place so all threads can access all variables and they have the common set of variable, common address space. This can be proved by looking at the output.

For the 2 processes, notice that the printed value of x is different. One is the original value (parent process), one has increment to 3 (child process). For the two threads, notice that only one of the thread is running this `addition_routine`. It increments the value of x. But, what happens is that the value of x is `3` on both thread.

Another differences is that, for threads, every threads share the file handlers whereas in processes, the file handlers are duplicated.

To summarize, the biggest difference between process and thread is that **thread shares memory while process don't**. This comes with a big tradeoff. Since threads are sharing memory, there could be some cases where multiple threads modifying the same variable, which is called as a **race condition** or **data race**.

### Race Conditions

```c
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;

void    *routine()
{
    for (int i = 0; i < 1000000; i++)
        mails++;
}

int main(void)
{
    pthread_t   p1, p2;

    if (pthread_create(&p1, NULL, &routine, NULL) != 0)
        return (1);
    if (pthread_create(&p2, NULL, &routine, NULL) != 0)
        return (2);
    if (pthread_join(p1, NULL) != 0)
        return (3);
    if (pthread_join(p2, NULL) != 0)
        return (4);
    printf("Number of mails: %d\n", mails);
    return (0);
}
```

In the example below, the created threads will increment the value of mails as their routine. Each thread will increment the value of mails, *1000000* times. Then we wait for the thread to done its execution using `pthread_join`. The program waited for each threads to finish its execution and print out the value of mails. As we mentioned earlier, all resources are shared between threads, so we can expect we get the value of mails as *2000000*? Bot of the thread increment the value for one million times right??

But no, you might get random value that is not *2000000*. If not, try increment the number of iteration and try again. If you do get some random number, this is what is called, a **race condition**, or **data race**. The two threads is accessing, modifying and updating the same value at the same time, which cause the value to be not expected.

Learn more about **race condition**: [Race condition explained](https://www.youtube.com/watch?v=K1aoimUYTK8)

> Race condition will only occurs on multi-core processor. If the processor is just a single core, it's most likely not gonna happen.

### Mutex in C

To solve a **race condition**, we can use what's called a **mutex**. A mutex is sort of a lock around a section of code and that section is often referred as a **critical section**. It protect that section of code being access by other threads at the same time. To initialize a mutex, use [`pthread_mutex_init`](#pthread_mutex_init). To destroy, use [`pthread_mutex_destroy`](#pthread_mutex_destroy).

```c
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex;

void    *routine()
{
    for (int i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(void)
{
    pthread_t   p1, p2;

    pthread_mutex_init(&mutex, NULL);
    if (pthread_create(&p1, NULL, &routine, NULL) != 0)
        return (1);
    if (pthread_create(&p2, NULL, &routine, NULL) != 0)
        return (2);
    if (pthread_join(p1, NULL) != 0)
        return (3);
    if (pthread_join(p2, NULL) != 0)
        return (4);
    pthread_mutex_destroy(&mutex);
    printf("Number of mails: %d\n", mails);
    return (0);
}
```

The example above is using mutex to lock this section of code so that only one thread can access it at a time.

```c
pthread_mutex_lock(&mutex); // protection start
mails++;
pthread_mutex_unlock(&mutex); // protection end
```

If we execute the code again, we can see that our output is 100% accurate. If you want, you can even create more thread and run the same routine and see if the output will be expected.

### Create threads in a loop

What's wrong with the loop below?

```c
int main(void)
{
    pthread_t   th[4];
    int i;

    for (i = 0; i < 4; i++) {
        if (pthread_create(th + i, NULL, &routine, NULL) != 0) {
            perror("Failed to create thread");
            return (1);
        }
        printf("Thread %d has started\n", i);
        if (pthread_join(th[i], NULL) != 0)
            return (2);
        printf("Thread %d has finished execution\n", i);
    }
}
```

If we execute the code above, the output message will be like this:

```text
Thread 0 has started
Thread 0 has finished exeuction
Thread 1 has started
Thread 1 has finished exeuction
Thread 2 has started
Thread 2 has finished exeuction
Thread 3 has started
Thread 3 has finished exeuction
```

It seems like we created the thread and wait for it to finishe its execution then only create another thread. We are not really running all the threads in parallel. There's no point of using multi-threads to run the program. So we shouldn't create and join the thread in the same loop. The correct way is to create another loop to join the threads like this:

```c
int main(void)
{
    pthread_t   th[4];
    int i;

    for (i = 0; i < 4; i++) {
        if (pthread_create(th + i, NULL, &routine, NULL) != 0) {
            perror("Failed to create thread");
            return (1);
        }
        printf("Thread %d has started\n", i);
    }
    for (i = 0; i < 4; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
            return (2);
        }
        printf("Thread %d has finished execution\n", i);
    }
}
```

If we try to execute the code, we will notice that the output is different now.

```text
Thread 0 has started
Thread 1 has started
Thread 2 has started
Thread 3 has started
Thread 0 has finished exeuction
Thread 1 has finished exeuction
Thread 2 has finished exeuction
Thread 3 has finished exeuction
```

### Get return value from a thread

```c
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

void    *roll_dice()
{
    int *result;
    
    result = malloc(sizeof(int))l
    if (result == NULL)
        return (NULL);
    *result = (rand() % 6) + 1;
    return ((void *)result);
}

int main(void)
{
    pthread_t   th;
    int         *res;

    srand(time(NULL));
    if (pthread_create(&th, NULL, &roll_dice, NULL) != 0)
        return (1);
    if (pthread_join(th, (void **)&res) != 0)
        return (2);
    printf("Result: %d\n", *res);
    free(res);
    return (0);
}
```

To get the return value from a thread, we need to use [`pthread_join`](#pthread_join) and it's has to do with the second parameter of the function. If we look at the function prototype, we can see that the data type of the second parameter is actually a `void **`. If we look at the function prototype of our routine, `roll_dice`, it actually will return a `void *`. So, the second parameter of `pthread_join` is a reference to a pointer and it's gonna set the value from the routine function we set.

Summary:

1. Declare a pointer to the data type you need.
2. Pass the pointer as a reference to the `pthread_join`'s second parameter. Typecast it to `void **` to avoid warning.
3. In routine function, set the value and typecast it to `void *` when returned.

### Pass arguments to threads in C

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/**
 * The problem:
 * 
 * Create 10 threads, each taking a unique prime from the primes array
 * and print it on the screen.
*/

int primes[10] = {2, 3, 5, 7, 11, 13 ,17, 19, 23, 29};

void    *print_prime(void *arg)
{
    int *num;

    sleep(1);
    num = arg;
    printf("%d\n", *num);
    return (NULL);
}

int main(void)
{
    pthread_t   th[10];

    for (int i = 0; i < 10; i++)
    {
        if (pthread_create(th + i, NULL, &print_prime, (void *)&primes[i]) != 0)
        {
            perror("Create thread failed!");
            return (1);
        }
    }
    for (int i = 0; i < 10; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
            return (2);
        }
    }
    return (0);
}

```

Summary:

1. Pass in the argument as a reference and typecast it to `void *`.
2. The only data type that the routine function could receive is a `void *`.
3. Typecast the `void *` to the corresponding data type inside the routine function.

### What are detached threads

A detached thread is no longer joinable we cannot wait for its execution before exiting the program. Another thing about detached thread is that they actually clear their own resources. `pthread_join` will also have the same effect. It waits for the thread to finish its execution and clear the resources.

One should call either `pthread_join` or `pthread_detach` at most once for each thread. Do not call both for the same thread. It's undefined behaviour.

### Deadlock

The causes of deadlock:

1. When a lock (mutex) has already been taken and if the program tries to lock it the second time, the program doesn't know that the lock is actually owned by the same thread and it's already locked.
2. When you have multiple locks (mutexes) and somehow they will be locked in different order in some condition. For example, mutex A will lock first then mutex B if condition is true. Else if condition B is false, mutex B lock first then mutex A. This will cause a deadlock where thread A lock mutex A first and wait for thread B to release mutex B so that it can lock it; same as thread B, it's waiting for thread A to release mutex A so that it can lock it.
