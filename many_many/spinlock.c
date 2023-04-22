#include "spinlock.h"

typedef struct myth_Node{
  int tid;
  int ktid;
  void *stack;
  void *args;
  int status;
  int timer;
  void * retval;
  void *(*f)(void *);
  ucontext_t context;
  struct spinlock * lk;
  struct myth_Node * next;
  struct myth_Node * prev;
} myth_Node;

extern myth_Node *head;

myth_Node * findNodektid(int ktid)
{
    myth_Node * t = head;
    do
    {
        if(t->status == 0 && t->ktid == ktid)
            return t;
        t = t->next;
    } while (t!=head);
    return NULL;
}

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
    while (__sync_lock_test_and_set(&lock->locked, 1) != 0)
    {
        tgkill(getpid(), findNodektid(gettid())->ktid, SIGALRM);
    }
}

void sleepunlock(struct spinlock *lock)
{
    __sync_lock_release(&lock->locked);
}
