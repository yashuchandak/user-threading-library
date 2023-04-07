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

int f1(void *arg) {
    int p = *(int *)arg;
    // printf("%d x= %d\n", p, x);
    
    // while(1) {    
    // }
    printf("thread1 running\n");

    for(int i=0; i<100000; i++) {
        printf("thread1 exiting\n");
    }

    thread_exit();
}

int f2(void *arg) {
    int p = *(int *)arg;
    // printf("%d x= %d\n", p, x);
    
    printf("thread2 running\n");

    for(int i=0; i<100000; i++) {
        printf("thread2 exiting\n");
    }

    thread_exit();
}

int main() {
    getcontext(&main_ctx);
    myth_t tid1, tid2, tid3;
    int arg1 = 10, arg2 = 20;

    thread_create(&tid1, f1, &arg1);
    thread_create(&tid2, f2, &arg2);
    thread_create(&tid3, f2, &arg2);

    sleep(1); 
    // printf("thread_id %d\n", tid);
    
    // int ret = thread_kill(tid, SIGINT);
    // printf("return value %d\n", ret);
    
    // if (ret == 0) {
    // 	printf("Thread killed\n");
    // }
    
    thread_join(tid1);
    thread_join(tid2);
    thread_join(tid3);

    return 0;
}

