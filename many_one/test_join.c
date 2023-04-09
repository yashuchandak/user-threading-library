#define _GNU_SOURCE
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>

#include "many_one.h"

ucontext_t main_ctx;

void *f(void *arg) {
    
    printf("in thread1\n");

    for(int k=0; k<110; k++) {
        printf("%d\n", k);        
    }

    // thread_exit();
    return 0;
}

int main() {
    
    getcontext(&main_ctx);
    myth_t tid;
    int arg = 10;

    thread_create(&tid, f, &arg);

    // sleep(1); 
    // printf("thread_id %d\n", tid);

    // thread_join(0);

    printf("last\n");

    return 0;
}

