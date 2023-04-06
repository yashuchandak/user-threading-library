#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ucontext.h>
#include <syscall.h>

#include "thread.h"

typedef struct node {
    myth_thread *thread; //structure?
    struct node *next;
}node;

typedef struct queue {
    node *head;
    node *rear;
}queue;

queue *q;
// = (queue *)malloc(sizeof(queue));
int tid = 0;

ucontext_t *cur_ctx;

void init() {
    q->head = q->rear = NULL;
}

void enqueue(myth_thread *thread) {
    node *nn = (node *)malloc(sizeof(node));
    nn->next = NULL;
    nn->thread = thread;

    if(!q->head) {
        q->head = q->rear = nn;
        return;
    }

    q->rear->next = nn;
    q->rear = nn;
}

myth_thread *dequeue(queue *q) {
    if(!q->head) {

    }
    node *temp = q->head;
    q->head = q->head->next;
    return temp;
}

// thread_create
myth_t thread_create(myth_t *thread, void *(*fn) (void *), void *args) {
  
    myth_thread thr;
    
    // node *nn = (node *)malloc(sizeof(node));
    thr.tid = tid;
    tid++;

    thr.args = args; 
    void *stack = malloc(4096);
    thr.stack = stack;
    thr.f = fn;

    getcontext(&(thr.context)); //?
    thr.context.uc_stack.ss_sp = stack; // sahi hai
    thr.context.uc_stack.ss_size = 4096;
    thr.context.uc_link = &main_ctx; //?
    makecontext(&(thr.context), *fn, 1, args); //?

    cur_ctx = &(thr.context);

    if(!q->head) {
        clone((int *)*fn , stack + 4096, 0, args);
    }
    else {
        enqueue(&thr);
    }

    return thr.tid;
}

int scheduler(int sched_case) {
    // manle sirf runnable aur running do state hai. ? //waiting
    // timer interrupt, runnable ki queue me se context uthao aur purana context q ke end pe dal do (swap context)
    // thread exit, queue me se vo node delete aur naya context lelo
    
    // get current context (with getcontext) and swap with below new context 
    
    //? //The getcontext() function initializes the structure pointed to by ucp to the current user context of the calling process.
    
    ucontext_t new = (dequeue(&q))->context;

    ucontext_t *temp = cur_ctx;
    cur_ctx = &new;

    if(sched_case == 1) { //timer interrupt
        swapcontext(temp, &new);
    }
    else if(sched_case == 2) { //thread_exit
        setcontext(&new);
    }
}

void thread_exit() {
    // ye thread ko list me se hata do, aur scheduler ko call karo
    // jo thread run ho rahi vo hi exit maregi

    scheduler(2);

}

int thread_join(myth_t thread) {
  printf("waitpid ret = %d\n", waitpid(thread, NULL, __WALL)); // wait for any child 
  return 0;
}