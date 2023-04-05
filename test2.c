#define _GNU_SOURCE
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h> 
#include "thread.h"
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>

ucontext_t main_ctx;
int x = 20;
myth_thread allthread[128];


int f(void *arg) {
    int p = *(int *)arg;
    printf("%d x= %d\n", p, x);
    
    while(1) {
        
    }

    thread_exit();
}

int main() {
    getcontext(&main_ctx);
    myth_t tid1, tid2;
    int arg1 = 10, arg2 = 11;

    thread_create(&tid1, f, &arg);
    thread_create(&tid2, f, &arg);

    sleep(1); 
    printf("thread_id %d\n", tid1); //printing twice? //locks?
    
    int ret = thread_kill(tid1, SIGINT);
    printf("return value %d\n", ret);
    
    if (ret == 0) {
    	printf("Thread killed\n");
    }
    
    thread_join(tid1);
    thread_join(tid2);

    return 0;
}

