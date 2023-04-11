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

void *f1(void *arg) {
    
    printf("thread1 running\n");

    // for(int i=0; i< 1000000; i++) {
    //     if(i%100 == 0) {
    //         printf("1 %d\n", i);
    //     }
    // }

    printf("thread1 exiting\n");

    thread_exit();
}

void *f2(void *arg) {
   
    printf("thread2 running\n");
   
    printf("thread2 exiting\n");

    thread_exit();
}

void *f3(void *arg) {
    printf("thread3 running\n");

    // for(int i=0; i<100000; i++) {
    //     if(i%100 == 0) {
    //         printf("3 %d\n", i);
    //     }
    // }
    printf("thread3 exiting\n");

    thread_exit();
}

void *f4(void *arg) {
    printf("thread4 running\n");

    // for(int i=0; i<100000; i++) {
    //     printf("4 %d\n", i);
    // }

    printf("thread4 exiting\n");

    thread_exit();
}

void *f5(void *arg) {
    printf("thread5 running\n");

    printf("thread5 exiting\n");

    thread_exit();
}



int main() {
    
    myth_t tid1, tid2, tid3, tid4, tid5;
    int arg1 = 10, arg2 = 20;

    thread_create(&tid1, f1, &arg1);
    thread_create(&tid2, f2, &arg2);
    

    // for(int j=0; j<100000; j++) {
        
    // }
    // printf("after wait in main\n");
    // printf("%daaaas\n", thread_kill(tid1, SIGKILL));

    thread_create(&tid3, f3, &arg1);
    thread_create(&tid4, f4, &arg2);
    thread_create(&tid5, f5, &arg1);
    // sleep(1); 
    
    // printf("%d\n", 
    // thread_kill(tid1, SIGTERM);
    

    thread_join(tid1);
    thread_join(tid2);
    thread_join(tid3);
    thread_join(tid4);
    thread_join(tid5);

    return 0;
}
