#include "many_one.h"
int tid = 0;
int isFirst = 0;
int mainFirst = 1;
myth_Node * head = NULL;
myth_Node * temp = NULL;
myth_Node * curr = NULL;
myth_Node * tail = NULL;

ucontext_t sch_ctx;
ucontext_t main_ctx;
struct itimerval timer;



struct spinlock tid_lk;  // for tid (global)
struct spinlock list_lk;  // for list (global)

extern void * main();

void init_all_locks()
{
    initlock(&tid_lk);
    initlock(&list_lk);
}

void begin_timer()
{
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 500; // 0.5 millisecs
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 500;
    setitimer(ITIMER_REAL, &timer, NULL);
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
    end_timer();
    curr->status = 1;
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
        
        myth_Node * nn = (myth_Node*)malloc(sizeof(myth_Node));
        nn->next = NULL;
        nn->prev = NULL;
        acquire(&tid_lk);
        nn->tid = tid++;
        release(&tid_lk);
        void *stack = malloc(4096);
        nn->stack = stack;
        nn->status = 1;
        nn->f = main;
        nn->lk = (struct spinlock*)malloc(sizeof(struct spinlock));
        initlock(nn->lk);
        nn->context = main_ctx;
        getcontext(&nn->context);
        nn->context.uc_stack.ss_sp = nn->stack;
        nn->context.uc_stack.ss_size = 4096;
        nn->context.uc_link = &sch_ctx; //?
        // makecontext(&(nn->context), (void(*)())main, 0);
        // getcontext(&(nn->context)); //correct?
        acquire(&list_lk);
        append(nn);
        release(&list_lk);
    }
    signal(SIGALRM, sig_alarm_handler);
    myth_Node * new = allocNode(thread,fn,args);
    getcontext(&(new->context)); 
    new->context.uc_stack.ss_sp = new->stack;
    new->context.uc_stack.ss_size = 4096;
    new->context.uc_link = &sch_ctx; 
    makecontext(&(new->context), (void(*)())fn, 1, args);
    
    
    acquire(&list_lk);
    append(new);
    release(&list_lk);
    if(!isFirst)
    {
        isFirst = 1;
        void * sch_stack = malloc(4096);
        scheduler();
    }
    return 0;
}

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

void cleanup(myth_Node *clean) {
    free(clean->stack);
    free(clean->args);
    free(clean->lk);
    free(clean);
}

myth_Node * checkRunable() {
    curr = temp;
    
    do {
        if (curr->next->status == 3) {
            if(curr->next==head)
            {
        

                head = head->next;
                tail->next = head;
                cleanup(curr->next);
            }
            else if(curr->next == tail)
            {

                myth_Node * del = curr->next;
                curr->next = head ;
                tail = curr;
                cleanup(del);
            }
            else
            {
                myth_Node * del = curr->next;
                curr->next = curr->next->next;
                del->next->prev = curr;
                cleanup(del);
            }
        }
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
        if(isAllTerminated())
        {
            return 0;
        }
        else
        {
            myth_Node * tnode = checkRunable();
            
            if(mainFirst == 1)
            {
                
                free(head->next);
                mainFirst ++;
                tid--;
                tail = head;
                head->next = head;
                head->prev = head;
            }
            begin_timer();
            swapcontext(&sch_ctx, &tnode->context); //timer int //1st isme current ctx save kardenga
        }
    }
    return 0;
}

void thread_exit(void * retptr)
{   
    end_timer();
    acquire(curr->lk);
    curr->status = -1;
    curr->retval = retptr;
    release(curr->lk);
    scheduler();
}

myth_Node * findNodeTid(int tid)
{
    myth_Node * t = head;
    do
    {
        if(t->tid == tid)
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
        acquire(curt->lk);
        curt->status = 2;  
        release(curt->lk);
        if(curt == curr)   
        {   
            swapcontext(&curt->context, &sch_ctx);
        }
    }
    
}

int thread_kill(int tid, int signal)
{   
    
    if( signal==SIGTERM || signal==SIGKILL || signal==SIGCONT || signal==SIGSTOP)
    {  
        if(!curr) {
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
                return -1;
            }
            handleSignal(sig_node, signal);
        }
        return 0;
    }
    return -1;
}

int thread_cancel(int tid)
{   
    if(!curr) {
        return -1;
    }
    if(curr->tid == tid)
    {   
        printf("curr node pe cancel\n");
        end_timer();
        curr->status = 3;
        swapcontext(&curr->context, &sch_ctx);
    }
    else
    {
        myth_Node * sig_node =  findNodeTid(tid);
        if(!sig_node) {
            return -1;
        }
        sig_node->status = 3; // cleanup status
    }
    return 0;
}

int thread_join(int tid,void ** value_ptr) {

    while (1)
    {
        myth_Node *tp = findNodeTid(tid);
        if(tp->status == -1) {
            if (value_ptr != NULL) {
                *value_ptr = tp->retval;
            }
            return 0;
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
