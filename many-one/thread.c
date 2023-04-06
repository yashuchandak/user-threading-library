#include <stdlib.h>
#include <ucontext.h>

#include "thread.h"


typedef struct node {
    myth_thread thread;
    struct node *next;
}node;

typedef struct queue {
    node *head;
    node *rear;
}queue;

void init(queue *q) {
    q->head = q->rear = NULL;
}

void enqueue(queue *q, myth_thread thread) {
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

//ready queue
queue q;

int scheduler(int sched_case) {
    // manle sirf runnable aur running do state hai. ? 
    // timer interrupt, runnable ki queue me se context uthao aur purana context q ke end pe dal do (swap context)
    // thread exit, queue me se vo node delete aur naya context lelo
    
    // get scheduler context and swap with below new context 
    ucontext_t new = (dequeue(&q))->context;

    if(sched_case == 1) { //timer interrupt
    }
    elif(sched_case == 2) { //thread_exit
    }
}

void thread_exit() {
    // ye thread ko list me se hata do, aur scheduler ko call karo
}

