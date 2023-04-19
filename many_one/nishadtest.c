#include "many_one.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <ucontext.h>
#include <stdlib.h>

#define size 3
int one = 1, two = 1;
int a[size][size];
int b[size][size];
int ans[size][size];

typedef struct mat {
	int r, c;
} mat;

void *mul (void* arg);
void view (void);


void *main () {
	int x = 0;
	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			a[i][j] = 0; b[i][j] = 0; ans[i][j] = 0;
		}
	}
	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			a[i][j] = one++; b[i][j] = two++;
		}
	}
	int tid[50];
	mat args[size * size];
	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			args[x].r = i; args[x].c = j;
			thread_create(&tid[x], mul, &args[x]);
			x++;
		}
	}
	for (int i=0; i<3; i++) {
		thread_join(i);
	}
	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			printf("%d ", ans[i][j]);
		}
		printf("\n");
	}
}

void *mul (void* arg) {
	
	mat* p = (mat*)arg;
	for (int i=0; i<size; i++) {
		ans[p->r][p->c] += a[p->r][i] * b[i][p->c];
	}
	
}

void view() {
	printf("%d\n",size);
	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			printf("%d ", ans[i][j]);
		}
		printf("\n");
	}
}
