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

void sig_alarm_handler(int sig);
extern ucontext_t main_ctx;
extern void * main();
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

myth_Node * allocNode(int *thread, void *(*fn) (void *), void *args);
void append(myth_Node * Node);
int thread_create(int *thread, void *(*fn) (void *), void *args);
void thread_exit(void *value_ptr);
int thread_join(int tid,void **value_ptr);
int thread_kill(int tid, int signal);
int thread_cancel(int tid);
void thread_mutex_lock(struct spinlock *mk);
void thread_mutex_unlock(struct spinlock *mk);
