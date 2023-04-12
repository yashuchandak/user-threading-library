#include "many_one.h"

int tid = 0;
int isFirst = 0;

myth_Node * head = NULL;
myth_Node * temp = NULL;
myth_Node * curr = NULL;
myth_Node * tail = NULL;

ucontext_t sch_ctx;
extern ucontext_t main_ctx;
struct itimerval timer;

struct spinlock tid_lk;  // for tid (global)
struct spinlock list_lk;  // for list (global)

void init_all_locks()
{
    initlock(&tid_lk);
    initlock(&list_lk);
}

void begin_timer()
{
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000; // 0.5 millisecs
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000;
    setitimer(ITIMER_REAL, &timer, NULL);
    // printf("%d\n", timer.it_value.tv_usec);
}

void end_timer()
{
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
    
}

void sig_alarm_handler(int sig) {
    // printf("In signal handler\n");
    end_timer();
    curr->status = 1;
    // scheduler();
    swapcontext(&curr->context, &sch_ctx);
}

myth_Node * allocNode(int *thread, void *(*fn) (void *), void *args)
{
    myth_Node * nn = (myth_Node*)malloc(sizeof(myth_Node));
    nn->next = NULL;
    nn->prev = NULL;
    acquire(&tid_lk);
    nn->tid = tid++;
    release(&tid_lk);
    *thread = nn->tid;
    nn->args = args;
    void *stack = malloc(4096);
    nn->stack = stack;
    nn->status = 1;
    nn->f = fn;
    nn->lk = (struct spinlock*)malloc(sizeof(struct spinlock));
    initlock(nn->lk);
    return nn;
}

void append(myth_Node * Node){
    if(!head)
    {
        head = Node;
        tail = Node;
        Node ->next = Node;
        temp = head;
    }
    else
    {
        tail->next = Node;
        Node->next = head;
        Node->prev = tail;
        tail = Node;
    }
}


int thread_create(int *thread, void *(*fn) (void *), void *args)
{
    if(!isFirst)
    {
        init_all_locks();

    }
    signal(SIGALRM, sig_alarm_handler);
    myth_Node * new = allocNode(thread,fn,args);
    getcontext(&(new->context)); //correct?
    new->context.uc_stack.ss_sp = new->stack;
    new->context.uc_stack.ss_size = 4096;
    new->context.uc_link = &sch_ctx; //?
    makecontext(&(new->context), (void(*)())fn, 1, args);
    acquire(&list_lk);
    append(new);
    release(&list_lk);
    if(!isFirst)
    {
        isFirst = 1;
        void * sch_stack = malloc(4096);
        clone(scheduler, sch_stack+4096, CLONE_VM, NULL); //aur falg use karne hai; main me se signal nhi ja rahe
        // scheduler();
    }
    return 0;
}

// thread create pehli bar call hua aur schedular function clone hua, lekin parent ne return kar diya main me, parent child ke liye nhi ruka; ab main aur scheduler do chal rahe, naya thread create aata to manage ho jata... .fir jb kabhi sab node terminated status mile to clone vali chiz exit status return maregi. baat khatam. 

int isAllTerminated()
{
    myth_Node * temp = head;
    do
    {
        if(temp->status!=-1)
            return 0;
        temp = temp->next;
    } while (temp!=head);
    return 1;
}

myth_Node * checkRunable() {
    curr = temp;
    do {
        if (curr->status == 1) {
            temp = curr->next;
            return curr;
        }
        curr = curr->next;
    } while (temp != curr);
    return NULL;
}

int scheduler()
{
    signal(SIGALRM, sig_alarm_handler);
    getcontext(&sch_ctx);
    while(1)
    {
        // printf("In sched %d\n", i);
        if(isAllTerminated())
        {
            // printf("Exit\n");
            // exit(0); 
            return 0;
        }
        else
        {
            myth_Node * tnode = checkRunable();
            begin_timer();
            swapcontext(&sch_ctx, &tnode->context); //timer int //1st isme current ctx save kardenga
        }
        // printf("before sig handler\n"); //sahi
    }
    return 0;
}

void thread_exit()
{   
    end_timer();
    acquire(curr->lk);
    curr->status = -1;
    release(curr->lk);
    futex_wake(&curr->status, 1);
}

myth_Node * findNodeTid(int tid)
{
    myth_Node * t = head;
    do
    {
        if(t->tid == tid)
            // printf()
            return t;
        t = t->next;
    } while (t!=head);
    return NULL;
}

void handleSignal(myth_Node * curt, int signal)
{
    
    if(signal == SIGTERM || signal == SIGKILL)
    {
        acquire(curt->lk);
        curt->status = -1;
        release(curt->lk);
        if(curt == curr) {
            // end_timer();
            swapcontext(&curt->context, &sch_ctx);
        }
    }
    else if(signal == SIGCONT)
    {
        acquire(curt->lk);
        curt->status = 1;
        release(curt->lk);
    }
    else if(signal == SIGSTOP)
    {
        // printf("sigstop\n");
        acquire(curt->lk);
        curt->status = 2;  //sigstop
        release(curt->lk);
        if(curt == curr)   //schedule next
        {   
            // end_timer();
            swapcontext(&curt->context, &sch_ctx);
        }
    }
    
}

int thread_kill(int tid, int signal)
{   
    
    if( signal==SIGTERM || signal==SIGKILL || signal==SIGCONT || signal==SIGSTOP)
    {  
        if(!curr) {
            // printf("df\n");
            return -1;
        }
        if(curr->tid == tid)
        {   
            end_timer();
            handleSignal(curr, signal);
        }
        else
        {
            myth_Node * sig_node =  findNodeTid(tid);
            if(!sig_node) {
                // printf("sig_node not found\n");
                return -1;
            }
            end_timer();
            handleSignal(sig_node, signal);
        }
        return 0;
    }
    // printf("Dffv\n");
    return -1;
}

int thread_join(int tid) {
    myth_Node *target_node = findNodeTid(tid);
    if (target_node == NULL) {
        return -1;
    }

    while (1) {
        if (target_node->status == -1) {
            return 0;
        }
        if (futex_wait(&target_node->status, 1) != 0) {
            if (errno != EINTR) {
                return -1;
            }
        }
    }
    
    return -1;
}

void thread_mutex_lock(struct spinlock * mk)
{
    sleeplock(mk);
}

void thread_mutex_unlock(struct spinlock *mk)
{
    sleepunlock(mk);
}
