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
int x = 0;
struct spinlock mk;
void *f1(void *arg) {
    
    int p = *(int *)arg;
    // printf("thread1 running\n");
    // printf("1 %d\n",p);
    
    for(int i=0;i<10000000;i++)
    {
        // printf("1 %d\n",x);
        
        thread_mutex_lock(&mk);
        x++;
        thread_mutex_unlock(&mk);
    }
    // printf("thread1 exiting\n");

    thread_exit();
}

void *f2(void *arg) {
    int p = *(int *)arg;
    // printf("thread2 running\n");
    // printf("2 %d\n",p);
    for(int i=0;i<10000000;i++)
    {
        // printf("2 %d\n",x);
        thread_mutex_lock(&mk);
        //printf("Hi 2\n");
        x++;
        thread_mutex_unlock(&mk);
    }
    // printf("thread2 exiting\n");

    thread_exit();
}

void *f3(void *arg) {
    int p = *(int *)arg;
    printf("thread3 running\n");
    printf("3 %d\n",p);
    // for(int i=0; i<100000; i++) {
    //     printf("3 %d\n", i);
    // }
    printf("thread3 exiting\n");

    thread_exit();
}

void *f4(void *arg) {

    int p = *(int *)arg;
    // printf("%d x= %d\n", p, x);
    
    printf("thread4 running\n");
    // thread_kill(0,SIGCONT);
    // for(int i=0; i<100000; i++) {
    //     printf("4 %d\n", i);
    // }
    printf("4 %d\n",p);
    printf("thread4 exiting\n");

    thread_exit();
}

void *f5(void *arg) {
    int p = *(int *)arg;
    // printf("%d x= %d\n", p, x);
    
    printf("thread5 running\n");
    printf("5 %d\n",p);
    // for(int i=0; i<100000; i++) {
        
    // }

    printf("thread5 exiting\n");

    thread_exit();
}



int main() {
    initlock(&mk);
    int tid1, tid2, tid3, tid4, tid5;
    int arg1 = 10, arg2 = 20;
    thread_create(&tid1, f1, &arg1);
    thread_create(&tid2, f2, &arg1);
    // thread_create(&tid3, f3, &arg1);
    
    // thread_create(&tid4, f4, &arg2);
    // thread_create(&tid5, f5, &arg1);

    for(int i=0;i<100000;i++)
    {

    }

    thread_join(0);
    
    thread_join(1);
    printf("%d\n",x);
    // thread_join(2);
    // thread_join(3);
    // thread_join(4);


    
    return 0;
}
