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

int i = 0;

void begin_timer()
{
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 500; // 0.5 millisecs
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 500;
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
    printf("In signal handler\n");
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
    nn->tid = tid++;
    nn->args = args;
    void *stack = malloc(4096);
    nn->stack = stack;
    nn->status = 1;
    nn->f = fn;
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


int thread_create(myth_t *thread, void *(*fn) (void *), void *args)
{
    signal(SIGALRM, sig_alarm_handler);
    myth_Node * new = allocNode(thread,fn,args);
    getcontext(&(new->context)); //correct?
    new->context.uc_stack.ss_sp = new->stack;
    new->context.uc_stack.ss_size = 4096;
    new->context.uc_link = &sch_ctx; //?
    makecontext(&(new->context), (void(*)())fn, 1, args);
    
    append(new);
    if(!isFirst)
    {
        isFirst = 1;
        void * sch_stack = malloc(4096);
        clone(scheduler,sch_stack+4096,CLONE_VM,NULL);
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
        printf("In sched %d\n", i);
        myth_Node * tnode = checkRunable();
        if(isAllTerminated())
        {
            printf("Exit\n");
            exit(0);
        }
        else
        {
            printf("elseme\n");
            begin_timer();
            swapcontext(&sch_ctx, &tnode->context); //timer int //1st isme current ctx save kardenga
        }
        // printf("before sig handler\n"); //sahi
    }
    return 0;
}

void thread_exit()
{   
    printf("2 in exit\n");
    end_timer();
    curr->status = -1;
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
        curt->status = -1;
        if(curt == curr) {
            // end_timer();
            printf("tk here\n");
            swapcontext(&curt->context, &sch_ctx);
        }
    }
    else if(signal == SIGCONT)
    {
        curt->status = 1;
    }
    else if(signal == SIGSTOP)
    {
        // printf("sigstop\n");
        curt->status = 2;  //sigstop
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
                printf("sig_node not found\n");
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
    // int ret = waitpid(thread, NULL, __WALL); // wait for any child 
    // 
    
    int ret = syscall(SYS_futex, &tid, FUTEX_WAIT, tid, NULL, NULL, 0);

    // myth_Node *tp = head;
    // do {
    //     if(tp->next->tid == tid && tp->next->status == -1) {
    //         tp->next = tp->next->next;
    //         if(tp->next == head) {
    //             head = head->next;
    //         }
    //         if(tp->next == tail) {
    //             tail = tp;
    //         }
    //         if(tp->next == tp) {
    //             head = tail = NULL;
    //         }
    //         free(tp->next);
    //         break;    
    //     } 
    //     tp = tp->next;    
    // }
    // while (tp->next != head);

    return ret;
}

