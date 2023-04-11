// Mutual exclusion lock.
#include <stdlib.h>


struct spinlock {
    int locked;
};

void initlock(struct spinlock *lk);
void acquire(struct spinlock *lk);
void release(struct spinlock *lk);

