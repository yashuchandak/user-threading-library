// Mutual exclusion spin locks.

#include "spinlock.h"

xchg(volatile uint *addr, uint newval)
{
  uint result;

  // The + in "+m" denotes a read-modify-write operand.
  asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");
  return result;
}


void initlock(struct spinlock *lk, char *name)
{
  lk->locked = 0;
}

void acquire(struct spinlock *lk)
{
  while(xchg(&lk->locked, 1) != 0)
    ;

  __sync_synchronize();

}

// Release the lock.
void release(struct spinlock *lk)
{
  __sync_synchronize();
  
  asm volatile("movl $0, %0" : "+m" (lk->locked) : );
}

