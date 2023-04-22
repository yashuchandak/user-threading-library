#include "many_many.h"
int num_of_kthread =2;
struct spinlock tid_lk;  // for tid (global)
struct spinlock list_lk;  // for list (global)
struct spinlock sch_lk;  // for scheduler
struct spinlock ktid_lk;  // for ktid
struct spinlock term_lk;
struct spinlock num_lk;
int tid =0;
ucontext_t sch_ctx;
myth_Node * head = NULL;
myth_Node * temp = NULL;
myth_Node * curr = NULL;
myth_Node * tail = NULL;

int num_of_uthread = 0;

int isFirst = 0;

struct itimerval timer[128];

void begin_timer(int i)
{
    timer[i].it_value.tv_sec = 0;
    timer[i].it_value.tv_usec = 500; // 0.5 millisecs
    timer[i].it_interval.tv_sec = 0;
    timer[i].it_interval.tv_usec = 500;
    setitimer(ITIMER_REAL, &timer[i], NULL);
}

void end_timer(int i)
{
    timer[i].it_value.tv_sec = 0;
    timer[i].it_value.tv_usec = 0;
    timer[i].it_interval.tv_sec = 0;
    timer[i].it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer[i], NULL);
}

myth_Node * findNodekTid(int ktid)
{
    myth_Node * t = head;
    do
    {
        if(t->status == 0 && t->ktid == ktid)
            return t;
        t = t->next;
    } while (t!=head);
    return NULL;
}

void sig_alarm_handler(int sig) {
    myth_Node *tp = findNodekTid(gettid());
    end_timer(tp->tid);
    if(tp->status == 0) {
        tp->status = 1;
    }
    swapcontext(&(tp->context), &sch_ctx);
}

void init_all_locks()
{
    initlock(&tid_lk);
    initlock(&list_lk);
    initlock(&sch_lk);
    initlock(&ktid_lk);
    initlock(&term_lk);
    initlock(&num_lk);

}

myth_Node * allocNode(int *thread, void *(*fn) (void *), void *args)
{
    myth_Node * nn = (myth_Node*)malloc(sizeof(myth_Node));
    nn->next = NULL;
    nn->prev = NULL;
    acquire(&tid_lk);
    nn->tid = tid++;
    nn->ktid = 0;
    release(&tid_lk);
    *thread = nn->tid;
    nn->args = args;
    void *stack = malloc(1024*1024);
    nn->stack = stack;
    nn->status = 1;
    nn->f = fn;
    nn->lk = (struct spinlock*)malloc(sizeof(struct spinlock));
    initlock(nn->lk);
    return nn;
}

void append(myth_Node * Node)
{
    if(!head)
    {
        head = Node;
        tail = Node;
        Node ->next = Node;
        temp = head->next;
    }
    else
    {
        tail->next = Node;
        Node->next = head;
        Node->prev = tail;
        tail = Node;
    }
}



void thread_exit(void *retptr)
{
    end_timer(findNodekTid(gettid())->tid);   
    myth_Node * exit_node = findNodekTid(gettid());
    acquire(exit_node->lk);
    exit_node->status = -1;
    exit_node->ktid = -1;
    exit_node->retval = retptr;
    release(exit_node->lk);
    setcontext(&sch_ctx);
}

myth_Node * checkRunable()
{
    myth_Node * tp = temp;
    do
    {
        if(tp->next->status==3)
        {
            if(tp->next==head)
            {
                head = head->next;
                tail->next = head;
                free(tp->next);
            }
            else if (tp->next == tail)
            {
                myth_Node * del = tp->next;
                tail = tp;
                tail->next = head;
                free(del);
            }
            else
            {
                tp->next = tp->next->next;
                free(tp->next);
            }
        }
        if(tp->status==1)
        {
            temp = tp->next;
            return tp;
        }
        tp= tp->next;
    } while (tp!=temp);
    return NULL;
}

int isAllTerminated()
{
    acquire(&term_lk);
    myth_Node * temp = head;
    do
    {
        if(temp->status == 1 || temp->status == 2 ) {
            release(&term_lk);
            return 0;
        }
        temp = temp->next;
    } while (temp!=head);
    release(&term_lk);
    return 1;
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

int thread_join(int tid, void **retptr) {
    while (1)
    {
        myth_Node *tp = findNodeTid(tid);
        if(tp->status == -1) {
            if(retptr!=NULL)
            {
                *retptr = tp->retval;
            }
            
            return 0;
        }
    }
}

void handleSignal(myth_Node * curt, int signal, int isCurrRunning)
{
    
    if(signal == SIGTERM || signal == SIGKILL)
    {
        if(isCurrRunning) {
            myth_Node * exit_node = curt;
            int temp = exit_node->ktid;

            acquire(exit_node->lk);
            exit_node->status = -1;
            exit_node->ktid = -1;
            release(exit_node->lk);
            tgkill(temp, temp, SIGALRM);
        }
        else
        {
            acquire(curt->lk);
            curt->status = -1;
            release(curt->lk);
        }
    }
    else if(signal == SIGCONT)
    {
        if(curt->status==-1)
            return ;
        acquire(curt->lk);
        curt->status = 1;
        release(curt->lk);
    }
    else if(signal == SIGSTOP)
    {
        acquire(curt->lk);
        if(curt->status==-1)
            return ;
        curt->status = 2;  //sigstop
        release(curt->lk);
        if(isCurrRunning)   //schedule next
        {   
            tgkill(curt->ktid,curt->ktid, SIGALRM);
        }
    }
    
}


int scheduler()
{
    getcontext(&sch_ctx);
    while (1)
    {
        if(isAllTerminated())
        {
            return 0;
        }
        else
        {
            acquire(&sch_lk);
            myth_Node * tnode = checkRunable();
            release(&sch_lk);
            if(tnode)
            {
                tnode->ktid = gettid();
                tnode->status = 0;
                begin_timer(tnode->tid);
                swapcontext(&sch_ctx, &tnode->context);
            }
        }
    }
    return 0;
}

int thread_create(int *thread, void *(*fn) (void *), void *args)
{
    if(!isFirst)
    {
        signal(SIGALRM, sig_alarm_handler);
        init_all_locks();
        begin_timer(0);  //main timer
        myth_Node * nn = (myth_Node*)malloc(sizeof(myth_Node));
        nn->next = NULL;
        nn->prev = NULL;
        acquire(&tid_lk);
        nn->tid = tid++;
        nn->ktid = gettid();
        release(&tid_lk);
        void *stack = malloc(4096);
        nn->stack = stack;
        nn->status = 0;
        nn->f = main;
        nn->lk = (struct spinlock*)malloc(sizeof(struct spinlock));
        initlock(nn->lk);
        
        getcontext(&nn->context);
        nn->context.uc_stack.ss_sp = nn->stack;
        nn->context.uc_stack.ss_size = 4096;
        makecontext(&(nn->context), (void(*)())main, 0);
        acquire(&list_lk);
        append(nn);
        release(&list_lk);
        isFirst = 1;
    }
    myth_Node * new = allocNode(thread,fn,args);
    getcontext(&(new->context)); 
    new->context.uc_stack.ss_sp = new->stack;
    new->context.uc_stack.ss_size = 4096;
    // new->context.uc_link = &sch_ctx; //?
    makecontext(&(new->context), (void(*)())fn, 1, args);
    acquire(&list_lk);
    append(new);
    release(&list_lk);
    if(num_of_uthread < num_of_kthread)
    {
        void * stack = malloc(1024*1024);
        new->status = 1;
        clone(scheduler,stack+1024*1024,CLONE_VM | CLONE_SIGHAND, args); // Add sigchild if neccessary
    }
    acquire(&num_lk);
    num_of_uthread++;
    release(&num_lk);
    return 0;
}

int thread_kill(int tid, int signal)
{   
    
    if( signal==SIGTERM || signal==SIGKILL || signal==SIGCONT || signal==SIGSTOP)
    {  
        acquire(&list_lk); 
        myth_Node * kill_node = findNodeTid(tid);
        release(&list_lk);

        if(kill_node->status == 0)
        {
            end_timer(kill_node->tid);
            handleSignal(kill_node, signal, 1);
        }
        else
        {
            handleSignal(kill_node,signal,0);
        }
        return 0;
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

int thread_cancel(int tid)
{   
    myth_Node * can_thr = findNodeTid(tid);
    acquire(can_thr->lk);
    can_thr->status = 3;
    release(can_thr->lk);
    tgkill(can_thr->ktid,can_thr->ktid,SIGALRM);
    return 0;
}