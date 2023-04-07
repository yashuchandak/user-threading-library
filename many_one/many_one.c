#include "many_one.h"


myth_t tid = 0;
extern ucontext_t main_ctx;
myth_Node * head = NULL;
myth_Node * tail = NULL;
 
ucontext_t *cur_ctx;

struct itimerval timer;

// alarm signal raise hone pe timer purane timer ko end aur scheuler
void sig_alarm_handler(int sig) {
    //scheduler ko call 
    printf("in timer handler\n");
    scheduler(1);
}

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

void append(myth_Node * Node){
    if(!head)
    {
        head = Node;
        tail = Node;
    }
    else
    {
        tail->next = Node;
        Node->prev = tail;
        tail = Node;
    }
}


myth_Node *delete() {
    if (!head) {
        return NULL;
    }
    myth_Node *temp = head;
    head = head->next;
    
    if (!head) {
        tail = NULL;
    }
    else
    {
        // head->prev = NULL;
    }

    return temp;
}

int first_create = 0;

// thread_create
myth_t thread_create(myth_t *thread, void *(*fn) (void *), void *args) {
    
    // signal(SIGALRM, sig_alarm_handler);

    myth_Node * nn = (myth_Node*)malloc(sizeof(myth_Node));
    nn->next = NULL;
    nn->prev = NULL;
    nn->tid = tid++;
    nn->args = args;
    void *stack = malloc(4096);
    nn->stack = stack;
    nn->f = fn;
  
    getcontext(&(nn->context)); //?
    nn->context.uc_stack.ss_sp = stack; // sahi hai
    nn->context.uc_stack.ss_size = 4096;
    nn->context.uc_link = &main_ctx; //?
    makecontext(&(nn->context), *fn, 1, args); //?
    
    cur_ctx = &(nn->context);

    if(!first_create) {
        first_create = 1;
        // begin timer
        begin_timer();
        clone((int *)*fn , stack + 4096, CLONE_VM, args);
    }
    else {
        // enqueue(&thr);
        append(nn);
        // printf("%d\n", q.head->thread->tid);
    }
    
    return nn->tid;
}

int scheduler(int sched_case) {

    //printf("in scheduler\n");
    
    // manle sirf runnable aur running do state hai. ? //waiting
    // timer interrupt, runnable ki queue me se context uthao aur purana context q ke end pe dal do (swap context)
    // thread exit, queue me se vo node delete aur naya context lelo
    
    // get current context (with getcontext) and swap with below new context 
    
    //? //The getcontext() function initializes the structure pointed to by ucp to the current user context of the calling process.
    
    if(!head) {
        exit(0); //main pura chalega?
    }
    else {
        myth_Node * tempNode = delete(); // idhar 4th node milegi
        
        // printf("%dhi\n", tempNode->tid);

        // apppended 4th node
        // tail->next = tempNode;
        // tail = tempNode;
        // tail->next = NULL;

        //append 3rd node
        myth_Node *pass = tempNode->prev;
        pass->next = pass->prev = NULL;
        
        // printf("%d tempnode->prev->tid\n", pass->tid);
        append(pass); 
        
        ucontext_t new = tempNode->context;
        ucontext_t *cur_ctx_copy = cur_ctx;
        cur_ctx = &new;

        if(sched_case == 1) { //timer interrupt
            // timer begin timer ki jarurat nhi, apne aap refresh?
            
            // swapcontext(cur_ctx_copy, &(tempNode->context));
            

            setcontext(&new);
        }
        else if(sched_case == 2) { //thread_exit
            // timer begin
            begin_timer();
            setcontext(&new);
        }
    }
}

void thread_exit() {
    // ye thread ko list me se hata do, aur scheduler ko call karo
    // jo thread run ho rahi vo hi exit maregi
    
    // timer end
    end_timer();

    scheduler(2);

}

int thread_join(myth_t thread) {
  int ret = waitpid(thread, NULL, __WALL); // wait for any child 
  return 0;
}





