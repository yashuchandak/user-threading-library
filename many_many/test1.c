// This file test thread_create, thread_exit, thread_join, timer interrupts and scheduler for a many many threading library..
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
    printf("Reading arguments given to thread1 %d\n",p);
    printf("Thread1 running on kernel thread id %d\n",gettid());
    for(int i=0;i<100000;i++)
    {
    }
    printf("thread1 exiting\n");
    thread_exit(0);
}

void *f2(void *arg) {
    printf("Thread2 running on kernel thread id %d\n",gettid());
    for(int i=0;i<100000;i++)
    {
    }
    printf("thread2 exiting\n"); 
    thread_exit(0);

}

void *f3(void *arg) {
    printf("Thread3 running on kernel thread id %d\n",gettid());
    for(int i=0;i<100000;i++)
    {
    }
    printf("thread3 exiting\n");
    thread_exit(0);
}

void *f4(void *arg) {
    printf("Thread4 running on kernel thread id %d\n",gettid());
    for(int i=0; i<100000; i++) {
    }
    
    printf("thread4 exiting\n");

    thread_exit(0);
}

void *f5(void *arg) {
    printf("Thread5 running on kernel thread id %d\n",gettid());
    for(int i=0; i<100000; i++) {
    }

    printf("thread5 exiting\n");

    thread_exit(0);
}

void *f6(void *arg) {
    printf("Thread6 running on kernel thread id %d\n",gettid());
    for(int i=0; i<100000; i++) {
    }

    printf("thread6 exiting\n");

    thread_exit(0);
}

void * main() {
    initlock(&mk);
    int tid1, tid2, tid3, tid4, tid5,tid6;
    int arg1 = 10;
    printf("Main running on kernel thread id %d\n",gettid());
    thread_create(&tid1, f1, &arg1);
    thread_create(&tid2, f2, NULL);
    thread_create(&tid3, f3, NULL);
    thread_create(&tid4, f4, NULL);
    thread_create(&tid5, f5, NULL);
    thread_create(&tid6, f6, NULL);

    for(int i=0;i<1000000;i++)
    {

    }

    thread_join(tid1,NULL);  
    thread_join(tid2,NULL);
    thread_join(tid3,NULL); 
    thread_join(tid4,NULL);  
    thread_join(tid5,NULL);  
    thread_join(tid6,NULL);  
    printf("Main Exiting\n");
    return NULL;
}