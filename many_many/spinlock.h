// Mutual exclusion lock.
#define _GNU_SOURCE
#include <signal.h>
#include <ucontext.h>
#include <stdlib.h>

struct spinlock {
    int locked;
};

void initlock(struct spinlock *lk);
void acquire(struct spinlock *lk);
void release(struct spinlock *lk);
void sleeplock(struct spinlock *lock);
void sleepunlock(struct spinlock *lock);