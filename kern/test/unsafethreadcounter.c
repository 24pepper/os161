#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

static int nthreads = 0;
static int counter = 0;
static int increment = 5;
static struct semaphore *kevsem = 0;

static
void
unsafethread(void *junk, unsigned long num)
{
        (void)junk;
        (void)num;

        for(int i = 0; i<increment; i++){
           counter++;
        }
        V(kevsem);
}

static
void
rununsafethread()
{
        char name[16];
        int i, result;

        kevsem = sem_create("kevsem", 0);

        for (i=0; i<nthreads; i++) {
                snprintf(name, sizeof(name), "unsafethreadcounter%d", i);
                result = thread_fork(name, NULL,
                                     unsafethread,
                                     NULL, i);
                if (result) {
                        panic("unsafe thread counter: thread_fork failed %s)\n", 
                              strerror(result));
                }
        }

        for (i=0; i<nthreads; i++) {
                P(kevsem);
        }

}

int
unsafethreadcounter(int nargs, char **args)
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

        kprintf("Starting the unsafe thread counter(you should see corruption occur)...\n");
        rununsafethread();
        sem_destroy(kevsem);
        kprintf("\nUnsafe thread counter done. Counter is %d should be %d\n",  counter, actualNum);

        return 0;
}

