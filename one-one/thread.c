#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ucontext.h>
#include "thread.h"
#include <syscall.h>

extern ucontext_t main_ctx;
int no = 0;


void wrapper(int (*fn) (void *), void *arg) {
  int ret = fn(arg);
}

myth_t thread_create(myth_t *thread, int (*fn) (void *), void *arg) {
  int tid;
  char *stack = malloc(4096);
  tid = clone(fn, stack + 4096, 0, arg);
  *thread = tid;
  printf("thread id = %d\n", *thread);

  getcontext(&allthread[no].context); //clone hone ke baad sidha run hone lagti. get context se ip set kiska thread vale func ka, apne aap change hota?
  allthread[no].context.uc_stack.ss_sp = stack;
  allthread[no].context.uc_stack.ss_size = 4096;
  allthread[no].context.uc_link = &main_ctx;
  // makecontext(&allthread[no].context, (void (*) (void)) wrapper, 2, fn, arg); // makecontext(&allthread[no].context, (void (*) (void)) wrapper, 2, fn, arg); //?  //jb agli bar ye context set ho to ue function chalao; lekin ye specify karne ki kya jarurat context ke ip pe se yahi run hone wala hai na? one one me jarurat nhi

  allthread[no].tid = tid;
  no++;
  return tid;
}

int thread_join(myth_t thread) {
  printf("waitpid ret = %d\n", waitpid(thread, NULL, __WALL));//wait for any child 
  return 0;
}

void thread_exit() {
  int cur_tid = gettid();  //?
  printf("Founded tid : %d\n",cur_tid);
  int index = -1;
  for (int i = 0; i < no; i++) {
    if (allthread[i].tid == cur_tid) {
      index = i;
      break;
    }
  }
  ucontext_t temp = allthread[index].context;
  if (index != -1) {
    free(allthread[index].context.uc_stack.ss_sp);

    for (int i = index; i < no - 1; i++) {
      allthread[i] = allthread[i+1];
    }
    no--;
    setcontext(&main_ctx); //?
  }
  else {
    exit(0); //?
  }
  
}

int thread_kill(int tid, int signal) {
  int index = -1;
  for (int i = 0; i < no; i++) {
    if (allthread[i].tid == tid) {
     // printf("All threads %d\n",allthread[i].tid);
      index = i;
      break;
    }
  }
  if (index == -1) {
    return -1; // couldnt find -1 is returned 
  }
  //printf("%d %d %d\n",tid,tgid,ntid);
  int tgid = tid; // for one one 
  int ret = tgkill(tgid, tid, signal);
  
  if (ret == -1) {
    perror("tgkill failed");
  }
  
  return ret;
}
