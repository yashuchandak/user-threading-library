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
    printf("args recieved thread1 running\n");
    for(int i=0;i<100000;i++)
    {
        
    }
    printf("thread1 exiting\n");
    thread_exit(0);
}

void *f2(void *arg) {
    printf("thread2 running\n");
    for(int i=0;i<100000;i++)
    {
    }
    printf("thread2 exiting\n");
    thread_exit(0);
}

void *f3(void *arg) {
    printf("thread3 running\n");
    for(int i=0;i<100000;i++)
    {

    }
    printf("thread3 exiting\n");

    thread_exit(0);
}

void *f4(void *arg) {
    printf("thread4 running\n");
    for(int i=0; i<100000; i++) {

    }
    
    printf("thread4 exiting\n");

    thread_exit(0);
}

void *f5(void *arg) {
    printf("thread5 running\n");
    for(int i=0; i<100000; i++) {

    }

    printf("thread5 exiting\n");
    thread_kill(3, SIGCONT);
    thread_exit(0);
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
    
    thread_kill(tid4, SIGTERM);
    thread_kill(tid3, SIGSTOP); 

    thread_join(tid1, NULL);  
    thread_join(tid2, NULL);
    thread_join(tid3, NULL); 
    thread_join(tid4, NULL);  
    thread_join(tid5, NULL);  
    
    printf("Done\n");
    return NULL;
}