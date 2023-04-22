#include "spinlock.h"
#include <stdlib.h>
#include <ucontext.h>

typedef struct myth_Node{
  int tid;
  void *stack;
  void *args;
  int status;
  int timer;
  void* retval;
  void *(*f)(void *);
  ucontext_t context;
  struct spinlock * lk;
  struct myth_Node * next;
  struct myth_Node * prev;
} myth_Node;

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

extern ucontext_t sch_ctx;
extern myth_Node *curr;

void sleeplock(struct spinlock *lock) 
{
    while(__sync_lock_test_and_set(&lock->locked, 1) != 0)
    {
        swapcontext(&curr->context, &sch_ctx);
    }
}

void sleepunlock(struct spinlock *lock)
{
    __sync_lock_release(&lock->locked);
}


