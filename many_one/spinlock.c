#include "spinlock.h"

void initlock(struct spinlock *lock)
{
    lock->locked = 0;
}

void acquire(struct spinlock *lock)
{
    while (__sync_lock_test_and_set(&lock->locked, 1) != 0)
    {
        
    }
}

void release(struct spinlock *lock)
{
    __sync_lock_release(&lock->locked);
}

void sleeplock(struct spinlock *lock)
{
    acquire(lock);

    while (__sync_lock_test_and_set(&lock->locked, 1) == 1)
    {
        futex_wait(&lock->locked, 1);
    }

    release(lock);
}

void sleepunlock(struct spinlock *lock)
{
    acquire(lock); // Acquire spinlock to protect mutex

    // Set lock_val to 0 to indicate that mutex is unlocked
    __sync_lock_release(&lock->locked);
    // Wake up one thread waiting on the mutex
    futex_wake(&lock->locked, 1);

    release(lock); // Release spinlock after releasing mutex
}