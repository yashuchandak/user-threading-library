#include "many_one.h"

void end_timer();
int tid = 0;
int isFirst = 0;
myth_Node *head = NULL;
myth_Node *temp = NULL;
myth_Node *curr = NULL;
myth_Node *tail = NULL;

ucontext_t sch_ctx;

struct itimerval timer;

void begin_timer()
{
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 100; // 0.5 millisecs
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 100;
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
        Node->next = Node;
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
    myth_Node * new = allocNode(thread, fn, args);
    getcontext(&(new->context)); //correct hmm?
    new->context.uc_stack.ss_sp = new->stack;
    new->context.uc_stack.ss_size = 4096;
    new->context.uc_link = &sch_ctx; 
    makecontext(&(new->context), (void(*)())fn, 1, args);
    
    append(new);
    if(!isFirst)
    {
        isFirst = 1;
        void * sch_stack = malloc(4096);
        clone(scheduler, sch_stack+4096, CLONE_VM, NULL);
    }
}

myth_Node * checkRunable ()
{
    curr = temp;
    do{
        if(curr->status == 1){
            temp = curr->next;
            return curr;
        }
        curr = curr->next;
    }while(temp != curr);
    
    return NULL;
}

int scheduler()
{
    
    getcontext(&sch_ctx);
    while(1)
    {
        printf("In scheduler\n");
        myth_Node * tnode = checkRunable();
        
        if(!tnode)
        {
            
        }
        else
        {
            begin_timer();
            swapcontext(&sch_ctx, &tnode->context); //timer int //1st isme current ctx save kardenga
        }
    }
}

void thread_exit()
{
    end_timer();
    curr->status = -1;
}

int thread_join(myth_t thread) {
  int ret = waitpid(thread, NULL, __WALL); // wait for any child 
  return 0;
}

int thread_kill(int tid, int signal) {
  
  
  
  return 0;
}
