#define _GNU_SOURCE
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h> 
#include "thread.h"
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>

ucontext_t main_ctx;
int x = 20;
myth_thread allthread[128];

// int f(void *arg) {
//     int p = *(int *)arg;
//     printf("%d x= %d\n", p, x);
//     // int tid = syscall(SYS_gettid);
//     thread_exit();
//     // pthread_exit(0);
//     // return 0;
//     // exit(0);
// }

// int main() {
//     getcontext(&main_ctx);
//     myth_t tid[3];
//     int args[3], j, k;

//     for(k = 0; k < 3; k++) {
//         args[k] = 10 + k;
//         thread_create(&tid[k], f, &args[k]);
//     }
//     // thread_exit();
//     // exit(0);
//     // pthread_exit(0);
//     thread_kill(tid[1], SIGSTOP);
//     for(k = 0; k < 3; k++) {
//         thread_join(tid[k]);
//     }
//     return 0;
// }

int f(void *arg) {
    int p = *(int *)arg;
    printf("%d x= %d\n", p, x);
    
    while(1) {
        
    }

    thread_exit();
}

int main() {
    getcontext(&main_ctx);
    myth_t tid;
    int arg = 10;

    thread_create(&tid, f, &arg);

    sleep(1); 
    printf("thread_id %d\n", tid);
    
    int ret = thread_kill(tid, SIGINT);
    printf("return value %d\n", ret);
    
    if (ret == 0) {
    	printf("Thread killed\n");
    }
    
    thread_join(tid);

    return 0;
}

