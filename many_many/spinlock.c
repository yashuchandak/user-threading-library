#include "spinlock.h"

typedef struct myth_Node{
  int tid;
  int ktid;
  void *stack;
  void *args;
  int status;
  int timer;
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

extern ucontext_t sch_ctx;
// extern myth_Node *curr;
void sleeplock(struct spinlock *lock) //?
{
    while (__sync_lock_test_and_set(&lock->locked, 1) != 0)
    {
        // scheduler();
        // setcontext(&sch_ctx);
        tgkill(getpid(), findNodektid(gettid())->ktid, SIGALRM);
    }
}

void sleepunlock(struct spinlock *lock)
{
    __sync_lock_release(&lock->locked);
}

// int futex_wait(int *uaddr, int val)
// {
//     return syscall(SYS_futex, uaddr, FUTEX_WAIT, val, NULL, NULL, 0);
// }

// int futex_wake(int *uaddr, int n)
// {
//     return syscall(SYS_futex, uaddr, FUTEX_WAKE, n, NULL, NULL, 0);
// }

// void initfutexlock(struct spinlock *lock)
// {
//     lock->locked = 0;
// }

// void sleeplock(struct spinlock *lock) //?
// {
//     while (__sync_lock_test_and_set(&lock->locked, 1) != 0)
//     {
//         int ret = futex_wait(&lock->locked,1);
//     }
// }

// void sleepunlock(struct spinlock *lock)
// {
//     __sync_lock_release(&lock->locked);
//     int ret = futex_wake(&lock->locked,1);
// }