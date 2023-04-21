// Mutual exclusion lock.
#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sched.h>
#include <linux/sched.h>
#include <sys/syscall.h>
#include <ucontext.h>
#include <sys/time.h>
#include <signal.h>
#include <linux/futex.h>
struct spinlock {
    int locked;
};

// extern ucontext_t sch_ctx;

void initlock(struct spinlock *lk);
void acquire(struct spinlock *lk);
void release(struct spinlock *lk);
int futex_wait(int *uaddr, int val);
int futex_wake(int *uaddr, int n);
void sleeplock(struct spinlock *lock);
void sleepunlock(struct spinlock *lock);