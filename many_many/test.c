// main pe thr3 chalra

#define _GNU_SOURCE
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h> 
#include "many_many.h"
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>

ucontext_t main_ctx;
int x = 0;
struct spinlock mk;
void *f1(void *arg) {

    // printf("thread1 running\n");
    printf("thread1 running on %d\n", gettid());
    printf("thread1 running on %d\n", gettid());
    // printf("thread1 running on %d\n", gettid());
    // printf("thread1 running on %d\n", gettid());
    // printf("thread1 running on %d\n", gettid());
    // printf("thread1 running on %d\n", gettid());

    // printf("thread1 running on %d\n", gettid());
    // printf("thread1 running on %d\n", gettid());
    // printf("thread1 running\n");
    // printf("thread1 running\n");
    // // alarm(0.05);
    for(int i=0; i<100000; i++)
    {
        printf("1 %d\n",i);
    }

    
    printf("thread1 exiting\n");
    printf("thread1 exiting\n");
    // printf("thread1 exiting\n");
    // printf("thread1 exiting\n");
    // printf("thread1 exiting\n");
    // printf("thread1 exiting\n");
    
    thread_exit();

    // thread_exit();
}

void *f2(void *arg) {
    // int p = *(int *)arg;

    // printf("thread2 running\n");
    // printf("thread2 running\n");
    printf("thread2 running on %d\n", gettid());
    printf("thread2 running on %d\n", gettid());
    // printf("thread2 running on %d\n", gettid());
    // printf("thread2 running on %d\n", gettid());
    // printf("thread2 running on %d\n", gettid());
    // printf("thread2 running on %d\n", gettid());
    // printf("thread2 running on %d\n", gettid());

    // printf("thread2 running on %d\n", gettid());
    
    // printf("thread2 running\n");
    // printf("thread2 running\n");

    for(int i=0; i<100000; i++)
    {
        printf("2 %d\n",i);
    }

    printf("thread2 exiting\n");
    printf("thread2 exiting\n");
    // printf("thread2 exiting\n");
    // printf("thread2 exiting\n");
    // printf("thread2 exiting\n");
    // printf("thread2 exiting\n");

    thread_exit();

    // thread_exit();
}

void *f3(void *arg) {
    // int p = *(int *)arg;

    // printf("thread3 running\n");
    // printf("thread3 running\n");
    printf("thread3 running on %d\n", gettid());
    printf("thread3 running on %d\n", gettid());
    // printf("thread3 running on %d\n", gettid());
    // printf("thread3 running on %d\n", gettid());
    // printf("thread3 running on %d\n", gettid());
    // printf("thread3 running on %d\n", gettid());

    // printf("thread3 running on %d\n", gettid());

    // printf("thread3 running\n");
    // printf("thread3 running\n");

    for(int i=0; i<10000; i++)
    {
        printf("3 %d\n",i);
    }
    
    printf("thread3 exiting\n");
    printf("thread3 exiting\n");
    // printf("thread3 exiting\n");
    // printf("thread3 exiting\n");
    // printf("thread3 exiting\n");
    // printf("thread3 exiting\n");

    thread_exit();

    // thread_exit();
}

void *f4(void *arg) {
    // printf("thread4 running\n");
    // printf("thread4 running\n");
    printf("thread4 running on %d\n", gettid());
    printf("thread4 running on %d\n", gettid());
    // printf("thread4 running on %d\n", gettid());
    // printf("thread4 running on %d\n", gettid());
    // printf("thread4 running on %d\n", gettid());
    // printf("thread4 running on %d\n", gettid());
    // printf("thread4 running on %d\n", gettid());

    
    for(int i=0; i<10000; i++)
    {
        printf("4 %d\n",i);
    }
    
    printf("thread4 exiting\n");
    printf("thread4 exiting\n");
    // printf("thread4 exiting\n");
    // printf("thread4 exiting\n");
    // printf("thread4 exiting\n");
    // printf("thread4 exiting\n");

    thread_exit();

    // thread_exit();
}

void *f5(void *arg) {

    printf("thread5 running on %d\n", gettid());
    printf("thread5 running on %d\n", gettid()); 
    for(int i=0; i<10000; i++)
    {
        printf("5 %d\n",i);
    }
    
    printf("thread5 exiting\n");
    printf("thread5 exiting\n");

    thread_exit();
}


void *f6(void *arg) {

    printf("thread6 running on %d\n", gettid());
    printf("thread6 running on %d\n", gettid());
    // printf("thread6 running on %d\n", gettid());
    // printf("thread6 running on %d\n", gettid());
    // printf("thread6 running on %d\n", gettid());
    // printf("thread6 running on %d\n", gettid());
    // printf("thread6 running on %d\n", gettid());

    
    for(int i=0; i<10000; i++)
    {
        printf("6 %d\n",i);
    }
    
    printf("thread6 exiting\n");
    printf("thread6 exiting\n");
    // printf("thread6 exiting\n");
    // printf("thread6 exiting\n");
    // printf("thread6 exiting\n");
    // printf("thread6 exiting\n");
    thread_exit();
}

void * main() {
    // initlock(&mk);
    int tid1, tid2, tid3, tid4, tid5 , tid6;
    int arg1 = 10, arg2 = 20;
    // signal(SIGALRM,sig_alarm_handler);
    printf("Main running on %d\n",gettid());
    thread_create(&tid1, f1, &arg1);
    thread_create(&tid2, f2, &arg2);
    thread_create(&tid3, f3, &arg2);
    thread_create(&tid4, f4, &arg1);
    thread_create(&tid5, f5, &arg1);
    thread_create(&tid6, f6, &arg1);

    // printf("thread kill calling\n");
    // thread_kill(1, SIGTERM);
    thread_join(tid1);
    thread_join(tid2);
    thread_join(tid3);
    thread_join(tid4);
    thread_join(tid5);
    thread_join(tid6);

    printf("Main finished\n");
    
    thread_exit();
}
