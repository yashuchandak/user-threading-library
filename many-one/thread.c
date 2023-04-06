#include <stdlib.h>

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

myth_thread dequeue(queue *q) {
    if(!q->head) {
        
    }

}



scheduler () {
    // manle sirf runnable aur running do state hai. ? 
    // timer interrupt, runnable ki queue me se context uthao aur purana context q ke end pe dal do
    // thread exit, queue me se vo node delete aur naya context lelo
}

void thread_exit() {
    // ye thread ko list me se hata do, aur scheduler ko call karo
}

