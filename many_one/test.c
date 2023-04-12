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
    // int p = *(int *)arg;
    printf("thread1 running\n");

    // printf("1 %d\n",p);
    for(int i=0;i<6000;i++)
    {
        
        // thread_mutex_lock(&mk);
        printf("1 %d\n",i);
        // acquire(&mk);
        x++;
        // release(&mk);
        // thread_mutex_unlock(&mk);
    }
    
    printf("thread1 exiting\n");

    thread_exit();
}

void *f2(void *arg) {
    // int p = *(int *)arg;
    printf("thread2 running\n");
    // printf("2 %d\n",p);
    // thread_kill(1, SIGSTOP);
    printf("Yash\n");
    for(int i=0;i<5000;i++)
    {
        
        // thread_mutex_lock(&mk);
        printf("yash %d\n",i+1);
        // acquire(&mk);
        x++;
        // release(&mk);
        // thread_mutex_unlock(&mk);
    }
    // thread_kill(1, SIGCONT);
    printf("thread2 exiting\n");

    thread_exit();
}

void *f3(void *arg) {
    int p = *(int *)arg;
    printf("thread3 running\n");
    printf("3 %d\n",p);
    for(int i=0;i<5000;i++)
    {
        
        // thread_mutex_lock(&mk);
        printf("3 %d\n",i);
        // acquire(&mk);
        x++;
        // release(&mk);
        // thread_mutex_unlock(&mk);
    }
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

// void *f6(void *arg) {
//     initlock(&mk);
//     int tid1, tid2, tid3, tid4, tid5;
//     int arg1 = 10, arg2 = 20;

//     thread_create(&tid1, f1, &arg);

//     thread_create(&tid2, f2, &arg);
    
//     // for(int i=0; i<1000; i++) {

//     // }
//     printf("dhvchv\n");
    
//     // thread_create(&tid4, f4, &arg2);
//     // thread_create(&tid5, f5, &arg1);

//     // for(int i=0;i<100000;i++)
//     // {

//     // }
    
//     thread_join(tid1);  
//     thread_join(tid2);
//     thread_join(tid3);


//     thread_exit();
// }





void * main() {
    initlock(&mk);
    int tid1, tid2, tid3, tid4, tid5;
    int arg1 = 10, arg2 = 20;
    thread_create(&tid1, f1, &arg1);
    thread_create(&tid2, f2, &arg2);
    thread_create(&tid3, f3, &arg2);
    
    // TIME CHAHIYE
    
    // for(int i=0; i<100000; i++) {

    // }
    // thread_kill(1, SIGSTOP);
    
    printf("TId %d\n",tid1);
    printf("TId %d\n",tid2);
    
    thread_join(1);  
    thread_join(2);
    thread_join(3);

    // TIME CHAHIYE; nhi to segfault

    // printf("hello\n");
    // for(int i=0; i<1000000; i++) {

    // }
    // sleep(1);
    
    return NULL;
}