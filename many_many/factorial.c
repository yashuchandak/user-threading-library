#include <stdio.h>
#include "many_many.h"

struct ThreadArgs {
    int  start;
    int  end;
};

void *factorial(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs*)arg;
    int  start = args->start;
    int  end = args->end;
    long long int  result = 1;
    for (int  i = start; i <= end; i++) {
        result *= i;
    }
    long long int  *res_ptr = malloc(sizeof(long long int ));
    *res_ptr = result;
    thread_exit(res_ptr);
}

void * main() {
    int  threads[4];
    struct ThreadArgs thread_args[4] = {{2, 5}, {6, 8}, {9, 14}, {15, 17}};
    long long int  results[4];
    

    for (int  i = 0; i < 4; i++) {
        thread_create(&threads[i], factorial, &thread_args[i]);
    }
    
    for (int  i = 0; i < 4; i++) {
        void *res_ptr;
        thread_join(threads[i], &res_ptr);
        long long int  *res = (long long int *)res_ptr;
        results[i] = *res;
        free(res);
    }
    long long int  final_result = 1;        
    for (int  i = 0; i < 4; i++) {
        final_result *= results[i];
    }
    printf("Final result: %lld\n", final_result);
    return NULL;
}
