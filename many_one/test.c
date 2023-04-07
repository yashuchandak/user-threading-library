#define _GNU_SOURCE
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h> 
#include "many_one.h"
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>

ucontext_t main_ctx;
int x = 20;

void f1(void *arg) {
    
    int p = *(int *)arg;
    // printf("%d x= %d\n", p, x);
    
    // while(1) {    
    // }
    printf("thread1 running\n");

    
    
    printf("thread1 exiting\n");

    thread_exit();
}

void f2(void *arg) {
    int p = *(int *)arg;
    // printf("%d x= %d\n", p, x);
    
    printf("thread2 running\n");

    for(int i=0; i<100000; i++) {
        
    }

    printf("thread2 exiting\n");

    thread_exit();
}

void f3(void *arg) {
    int p = *(int *)arg;
    // printf("%d x= %d\n", p, x);
    
    printf("thread3 running\n");

    // while (1)
    // {
    //     /* code */
    // }

    printf("thread3 exiting\n");

    thread_exit();
}

void f4(void *arg) {
    int p = *(int *)arg;
    // printf("%d x= %d\n", p, x);
    
    printf("thread4 running\n");

    for(int i=0; i<100000; i++) {
        
    }

    printf("thread4 exiting\n");

    thread_exit();
}



int main() {
    getcontext(&main_ctx);
    myth_t tid1, tid2, tid3  ,tid4;
    int arg1 = 10, arg2 = 20;

    thread_create(&tid1, f1, &arg1);
    thread_create(&tid2, f2, &arg2);
    thread_create(&tid3, f3, &arg1);
    thread_create(&tid4, f4, &arg2);
    // sleep(1); 
    
    
    thread_join(tid1);
    thread_join(tid2);
    thread_join(tid3);
    thread_join(tid4);

    return 0;
}

