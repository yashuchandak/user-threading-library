//This file tests the thread_kill for manymany thread mapping..
#define _GNU_SOURCE
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h> 
#include "many_many.h"
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>

int x = 0;
struct spinlock mk;
void *f1(void *arg) {
    int p = *(int *)arg;
    printf("\nargs recieved in p %d\n", p);
    printf("thread1 running\n");
    for(int i=0;i<1000000;i++)
    {
        
    }
    printf("thread1 exiting\n");
    thread_exit();
}

void *f2(void *arg) {
    printf("thread2 running\n");
    // thread_cancel(1);
    for(int i=0;i<100000;i++)
    {
    }
    printf("thread2 exiting\n");
    thread_exit();
}

void *f3(void *arg) {
    printf("thread3 running\n");
    for(int i=0;i<10000;i++)
    {

    }
    printf("thread3 exiting\n");

    thread_exit();
}

void *f4(void *arg) {
    printf("thread4 running\n");
    for(int i=0; i<100000; i++) {

    }
    
    printf("thread4 exiting\n");

    thread_exit();
}

void *f5(void *arg) {
    printf("thread5 running\n");
    for(int i=0; i<100000; i++) {

    }

    printf("thread5 exiting\n");

    thread_exit();
}

void * main() {

    printf("Main started\n");

    initlock(&mk);
    int tid1, tid2, tid3, tid4, tid5;
    int arg1 = 10;
    thread_create(&tid1, f1, &arg1);
    thread_create(&tid2, f2, NULL);
    thread_create(&tid3, f3, NULL);
    thread_create(&tid4, f4, NULL);
    thread_create(&tid5, f5, NULL);
    

    for(int i=0; i<200000; i++) {

    }
    thread_cancel(tid1);
      
    thread_join(tid2);
    thread_join(tid3); 
    thread_join(tid4);  
    thread_join(tid5);  

    printf("Done\n");

    thread_exit();
}
