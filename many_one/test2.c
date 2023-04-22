//This file tests the thread_kill for manyone thread mapping..
#define _GNU_SOURCE
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h> 
#include "many_one.h"
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>

int x = 0;
struct spinlock mk;
void *f1(void *arg) {
    int p = *(int *)arg;
    printf("Reading arguments given to thread1 %d\n",p);
    printf("thread1 running\n");
    for(int i=0;i<1000000;i++)
    {

    }
    printf("thread1 exiting\n");
    thread_exit(0);
}

void *f2(void *arg) {
    printf("thread2 running\n");
    for(int i=0;i<1000000;i++)
    {
    }
    printf("thread2 exiting\n");
    thread_exit(0);
}

void *f3(void *arg) {
    printf("thread3 running\n");
    for(int i=0;i<1000000;i++)
    {

    }
    printf("thread3 exiting\n");

    thread_exit(0);
}

void *f4(void *arg) {
    printf("thread4 running\n");
    for(int i=0; i<1000000; i++) {

    }
    
    printf("thread4 exiting\n");

    thread_exit(0);
}

void *f5(void *arg) {
    printf("thread5 running\n");
    for(int i=0; i<1000000; i++) {

    }

    printf("thread5 exiting\n");

    thread_exit(0);
}

void * main() {
    initlock(&mk);
    int tid1, tid2, tid3, tid4, tid5;
    int arg1 = 10;
    thread_create(&tid1, f1, &arg1);
    thread_create(&tid2, f2, NULL);
    thread_create(&tid3, f3, NULL);
    thread_create(&tid4, f4, NULL);
    thread_create(&tid5, f5, NULL);
    
    for(int i=0; i<1000000; i++) {

    }
    thread_kill(tid1, SIGTERM);
    
    thread_kill(tid3, SIGSTOP);
    printf("betn sigstop and sigcont\n");
    for(int i=0; i<1000000; i++) {  //delaying main thread to create and run threads before sending signals.
    
    }
    thread_kill(tid3, SIGCONT);
    thread_join(tid1,NULL);  
    thread_join(tid3,NULL); 
    thread_join(tid4,NULL);  
    thread_join(tid5,NULL);  
    return NULL;
}