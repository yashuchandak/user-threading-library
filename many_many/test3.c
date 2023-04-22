// This file tests the mutexlocks for manymany thread mapping..
#define _GNU_SOURCE
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h> 
#include "many_many.h"
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>

int x = 0,x1 = 0,x2 = 0;
int z = 0, z1 = 0, z2 = 0;
struct spinlock mk;

void *f1(void *arg) {
    for(int i=0;i<45000;i++)
    {
        thread_mutex_lock(&mk);
        x++;
        thread_mutex_unlock(&mk);
        x1++;
    }
    thread_exit(0);
}

void *f2(void *arg) {
    for(int i=0;i<45000;i++)
    {
        thread_mutex_lock(&mk);
        x++;
        thread_mutex_unlock(&mk);
        x2++;
    }
    thread_exit(0);
}

void *f3(void *arg) {
    for(int i=0;i<45000;i++)
    {
        z++;
        z1++;
    }
    thread_exit(0);
}



void *f4(void *arg) {
    for(int i=0; i<45000; i++) 
    {
       z++;
       z2++;
    }
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

    
    thread_join(tid1,NULL);  
    thread_join(tid2,NULL);
    thread_join(tid3,NULL); 
    thread_join(tid4,NULL); 
    
    printf("With mutex lock\n");
    printf("x = %d ; x1 = %d ; x2 = %d ; x1+x2 = %d\n", x, x1, x2, x1+x2); 
    printf("\nWithout mutex lock\n");
    printf("z = %d ; z1 = %d ; z2 = %d ; z1+z2 = %d\n", z, z1, z2, z1+z2);
    
    return NULL;
}