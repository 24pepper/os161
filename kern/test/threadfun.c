#include <types.h>
#include <lib.h>
#include <thread.h>
#include <synch.h>
#include <test.h>

static struct semaphore *tsem = NULL;
static int nthreads = 0;

static
void
init_semfun(void)
{
        if (tsem==NULL) {
                tsem = sem_create("tsem", 0);
                if (tsem == NULL) {
                        panic("threadtest: sem_create failed\n");
                }
        }
}

static
void
funkythread(void *junk, unsigned long num)
{
        V(tsem);
	int ch = '0' + num;
        (void)junk;

        putch(ch);
//	kprintf("%lu\n", num);

       // V(tsem);
}

static
void
runfunthread()
{
        char name[16];
        int i, result;

        for (i=0; i<nthreads; i++) {
                snprintf(name, sizeof(name), "threadtest%d", i);
                result = thread_fork(name, NULL,
                                     funkythread,
                                     NULL, i);
                if (result) {
                        panic("threadtest: thread_fork failed %s)\n", 
                              strerror(result));
                }
        }

        for (i=0; i<nthreads; i++) {
                P(tsem);
        }
}

int
mythreadtest(int nargs, char **args)
{
        (void)nargs;
        (void)args;

	nthreads = atoi(args[1]);

        init_semfun();
        kprintf("Starting the fun thread test...\n");
        runfunthread();
        kprintf("\nFun thread test done.\n");

        return 0;
}

