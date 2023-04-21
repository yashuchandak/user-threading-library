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
#include "spinlock.h"

typedef struct myth_Node{
  int tid;
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

int thread_create(int *thread, void *(*fn) (void *), void *args);
int scheduler();
void thread_exit() ;
int thread_join(int thread);
int thread_kill(int tid, int signal);
void thread_mutex_lock(struct spinlock * mk);
void thread_mutex_unlock(struct spinlock * mk);
int thread_cancel(int tid);
