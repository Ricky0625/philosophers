# Philosophers

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
5. A message announcing a philosopher died should be displaued no more than **10 ms** after the actual death of the philosopher.
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

If either `tv` or `tz` is *NULL*, the corresponding structure is not set or returned.

`gettimeofday` return `0` for success. On error, `-1` is returned and errno is set to indicate the error.

### `pthread_create`

```c
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
```

The `pthread_create` function is used to create a new thread, with attributes specified by `attr`, within a process. If `attr` is **NULL**, the default attributes are used. If the attributes specified by `attr` are modified later, the thread's attributes are not affected. Upon susccessful completion, `pthread_create` stores the ID of the created thread in the location referenced by `thread`.

### `pthread_detach`

### `pthread_join`

### `pthread_mutex_init`

### `pthread_mutex_destroy`

### `pthread_mutex_lock`

### `pthread_mutex_unlock`

### `fork` (bonus)

### `kill` (bonus)

### `waitpid` (bonus)

### `sem_open` (bonus)

### `sem_close` (bonus)

### `sem_post` (bonus)

### `sem_unlink` (bonus)
