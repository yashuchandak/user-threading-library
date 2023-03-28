#define _GNU_SOURCE
#include <sys/syscall.h>
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h> 
#include "thread.h"
#include <pthread.h>

ucontext_t main_ctx;
int x = 20;
myth_thread allthread[128];

int f(void *arg) {
    int p = *(int *)arg;
    // printf("%d x= %d\n", p, x);
    // printf("Hello\n");
    for(int i=0;i<5000000;i++)
    {
        
    }
    printf("func printing\n");
    // int tid = syscall(SYS_gettid);
    // thread_exit();
    // pthread_exit(0);
    return 0;
    // exit(0);
}

int main() {
    getcontext(&main_ctx);

    myth_t tid[3];
    int args[3], j, k;

    for(k = 0; k < 4; k++) {
        args[k] = 10 + k;
        thread_create(&tid[k], f, &args[k]);
    }
    int tid1 = syscall(SYS_gettid);
    
    // sleep(1);
    printf("main exiting");
    thread_exit();

    printf("print nhi hona chahiye");

    for(k = 0; k < 4; k++) {
        thread_join(tid[k]);
    }

    return 0;
}
