typedef int myth_t;

typedef struct {
  myth_t tid;
  void *args;
  void *stack;
  void *(*f)(void *); //void *(*f)?
  ucontext_t context;
} myth_thread;

extern ucontext_t main_ctx;

extern myth_thread allthread[128];
int thread_create(myth_t *thread, void *(*fn) (void *), void *arg);
int thread_join(myth_t thread);
void thread_exit();
int thread_kill(myth_t thread,int signal);