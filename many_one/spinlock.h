// Mutual exclusion lock.
struct spinlock {
    int locked;
};

// extern ucontext_t sch_ctx;

void initlock(struct spinlock *lk);
void acquire(struct spinlock *lk);
void release(struct spinlock *lk);
void sleeplock(struct spinlock *lock);
void sleepunlock(struct spinlock *lock);
