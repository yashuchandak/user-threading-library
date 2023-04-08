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

typedef int myth_t;

typedef struct myth_Node{
  myth_t tid;
  void *stack;
  void *args;
  int status;
  int timer;
  void *(*f)(void *);
  ucontext_t context;
  struct myth_Node * next;
  struct myth_Node * prev;
} myth_Node;

typedef struct mythList{
  myth_Node * head;
  myth_Node * tail;
} mythList;
extern ucontext_t main_ctx;

void append(myth_Node * Node);
myth_Node * traverse(ucontext_t temp);
myth_Node *  delete();
myth_t thread_create(myth_t *thread, void *(*fn) (void *), void *args);
void scheduler() ;
void thread_exit() ;
int thread_join(myth_t thread);
void sig_alarm_handler(int sig);
