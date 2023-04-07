#include "many_one.h"

static myth_t tid = 0;  //global
extern ucontext_t main_ctx;
myth_Node * head = NULL;
myth_Node * tail = NULL;
 
ucontext_t *cur_ctx;


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

void traverse()
{
    while (head)
    {
        head = head->next;
    }
    
}

myth_Node *delete() {
  if (!head) {
    return NULL;
  }
  myth_Node *temp = head;
  head = head->next;
  if (head) {
    head->prev = NULL;
  } else {
    tail = NULL;
  }
  return temp;
}

int first_create = 0;

// thread_create
myth_t thread_create(myth_t *thread, void *(*fn) (void *), void *args) {
  
    myth_Node * nn = (myth_Node*)malloc(sizeof(myth_Node));
    nn->next = NULL;
    nn->prev = NULL;
    nn->tid = tid++;
    nn->args = args;
    void *stack = malloc(4096);
    nn->stack = stack;
    nn->f = fn;
    // node *nn = (node *)malloc(sizeof(node));
    // thr.tid = tid;
    // tid++;

    // thr.args = args; 
    // void *stack = malloc(4096);
    // thr.stack = stack;
    // thr.f = fn;

    getcontext(&(nn->context)); //?
    nn->context.uc_stack.ss_sp = stack; // sahi hai
    nn->context.uc_stack.ss_size = 4096;
    nn->context.uc_link = &main_ctx; //?
    makecontext(&(nn->context), *fn, 1, args); //?
    
    cur_ctx = &(nn->context);

    if(!first_create) {
        first_create = 1;
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
    ucontext_t new = delete()->context;
    //  printf("%d\n",delete()->tid);
    ucontext_t *temp = cur_ctx;
    cur_ctx = &new;

    // printf("in scheduler\n");

    if(sched_case == 1) { //timer interrupt
       // printf("sched_case1\n");
        swapcontext(temp, &new);
    }
    else if(sched_case == 2) { //thread_exit
        //printf("from exit\n");
        setcontext(&new);
    }
}

void thread_exit() {
    // ye thread ko list me se hata do, aur scheduler ko call karo
    // jo thread run ho rahi vo hi exit maregi
    scheduler(2);

}

int thread_join(myth_t thread) {
  waitpid(thread, NULL, __WALL); // wait for any child 
  return 0;
}





