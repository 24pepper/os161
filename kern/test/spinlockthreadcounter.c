#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>
#include <spinlock.h>

static int nthreads = 0;
static int counter = 0;
static int increment = 5;
static struct semaphore *kevisem = 0;
static struct  spinlock kevinspinlock;


static void cleanitems(void){
    spinlock_cleanup(&kevinspinlock);
    sem_destroy(kevisem);
}

static
void
spinthread(void *junk, unsigned long num)
{
        (void)junk;
        (void)num;

        for(int i = 0; i<increment; i++){
           spinlock_acquire(&kevinspinlock);
           counter++;
           spinlock_release(&kevinspinlock);
        }
        V(kevisem);

}

static
void
runspinthread()
{
        char name[16];
        int i, result;

        spinlock_init(&kevinspinlock);
        kevisem = sem_create("kevisem", 0);

        for (i=0; i<nthreads; i++) {
                snprintf(name, sizeof(name), "spinlockthreadcounter%d", i);
                result = thread_fork(name, NULL,
                                    spinthread,
                                     NULL, i);
                if (result) {
                        panic("lock thread counter: thread_fork failed %s)\n",
                              strerror(result));
                }
        }

        for (i=0; i<nthreads; i++) {
                P(kevisem);
        }

}

int
spinlockthreadcounter(int nargs, char **args)
{
        (void)nargs;
        (void)args;
        counter = 0;

        if(nargs < 2){
          kprintf("No argument given. Must give number of threads\n");
          return 0;
        }

        nthreads = atoi(args[1]);

        if(nargs >= 3){
          increment = atoi(args[2]);
        }
        else{
          kprintf("No second argument given. Defaulting to 5 for the number of times threads increment the counter \n");
        }


        int actualNum = nthreads * increment;

        kprintf("Starting the spin lock thread counter...\n");
        runspinthread();
        cleanitems();
        kprintf("\nspin lock thread counter done. Counter is %d should be %d\n", counter, actualNum);

        return 0;
}

