#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

static int nthreads = 0;
static int counter = 0;
static int increment = 5;
static struct lock *kevinLock = 0;
static struct semaphore *kevinsem = 0;


static void cleanitems(void){
    lock_destroy(kevinLock);
    sem_destroy(kevinsem);
}

static
void
lockthread(void *junk, unsigned long num)
{
        (void)junk;
        (void)num;

        for(int i = 0; i<increment; i++){
           lock_acquire(kevinLock);
           counter++;
           lock_release(kevinLock);
        }
        V(kevinsem);

}

static
void
runlockthread()
{
        char name[16];
        int i, result;

        kevinLock = lock_create("kevinLock");
        kevinsem = sem_create("kevinsem", 0);

        for (i=0; i<nthreads; i++) {
                snprintf(name, sizeof(name), "lockthreadcounter%d", i);
                result = thread_fork(name, NULL,
                                    lockthread,
                                     NULL, i);
                if (result) {
                        panic("lock thread counter: thread_fork failed %s)\n",
                              strerror(result));
                }
        }

        for (i=0; i<nthreads; i++) {
                P(kevinsem);
        }

}

int
lockthreadcounter(int nargs, char **args)
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

        kprintf("Starting the lock thread counter...\n");
        runlockthread();
//        if(lock_do_i_hold(kevinLock)){
//          kprintf("you're still holding the damn lock\n");
//        }
//        else{
//         kprintf("you're not holding the lock\n");
//        }
        cleanitems();
        kprintf("\nlock thread counter done. Counter is %d should be %d\n", counter, actualNum);

        return 0;
}

