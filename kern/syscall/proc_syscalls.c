#include <types.h>
#include <kern/errno.h>
#include <kern/unistd.h>
#include <kern/wait.h>
#include <lib.h>
#include <syscall.h>
#include <current.h>
#include <proc.h>
#include <thread.h>
#include <addrspace.h>
#include <copyinout.h>
#include <mips/trapframe.h>

  /* this implementation of sys__exit does not do anything with the exit code */
  /* this needs to be fixed to get exit() and waitpid() working properly */

void sys__exit(int exitcode) {

  struct addrspace *as;
  struct proc *p = curproc;
  /* for now, just include this to keep the compiler from complaining about
     an unused variable */
  (void)exitcode;

  DEBUG(DB_SYSCALL,"Syscall: _exit(%d)\n",exitcode);

  KASSERT(curproc->p_addrspace != NULL);
  as_deactivate();
  /*
   * clear p_addrspace before calling as_destroy. Otherwise if
   * as_destroy sleeps (which is quite possible) when we
   * come back we'll be calling as_activate on a
   * half-destroyed address space. This tends to be
   * messily fatal.
   */
  as = curproc_setas(NULL);
  as_destroy(as);

  /* detach this thread from its process */
  /* note: curproc cannot be used after this call */
  proc_remthread(curthread);

  /* if this is the last user process in the system, proc_destroy()
     will wake up the kernel menu thread */
  proc_destroy(p);
  thread_exit();
  /* thread_exit() does not return, so we should never get here */
  panic("return from thread_exit in sys_exit\n");
}


/* stub handler for getpid() system call                */
int
sys_getpid(pid_t *retval)
{
  /* for now, this is just a stub that always returns a PID of 1 */
  /* you need to fix this to make it work properly */
  *retval = 1;
  return(0);
}

/* stub handler for waitpid() system call                */

int
sys_waitpid(pid_t pid,
	    userptr_t status,
	    int options,
	    pid_t *retval)
{
  int exitstatus;
  int result;

  /* this is just a stub implementation that always reports an
     exit status of 0, regardless of the actual exit status of
     the specified process.   
     In fact, this will return 0 even if the specified process
     is still running, and even if it never existed in the first place.

     Fix this!
  */

  if (options != 0) {
    return(EINVAL);
  }
  /* for now, just pretend the exitstatus is 0 */
  exitstatus = 0;
  result = copyout((void *)&exitstatus,status,sizeof(int));
  if (result) {
    return(result);
  }
  *retval = pid;
  return(0);
}

/* stub handler for the fork() system call (created by Kevin Beshears)*/

int sys_fork(struct trapframe *tf, pid_t *retval){
  struct trapframe * temp_tf;
  DEBUG(DB_SYSCALL,"Syscall: sys_fork()\n");
  int err;
  struct proc *proc;
  struct addrspace * child_vmspace = NULL;

  KASSERT(child_vmspace == NULL);
  as_copy(curproc->p_addrspace, &child_vmspace);
  if(child_vmspace == NULL){
  	kprintf("sys_fork: as_copy failed %s\n",strerror(ENOMEM));
        return ENOMEM;
  }

  proc = proc_create_fork("child");
  proc->p_addrspace = child_vmspace;
  proc->p_pid = 1;
  temp_tf = kmalloc(sizeof(tf));
  temp_tf = tf;

  err = thread_fork("test",proc,uproc_thread,temp_tf,1);

  if(err){
  	return err;
  }

 //for(int i = 0; i < 100000; ++i){

   // }

  kprintf("Parent returning after thread fork\n");

  //*retval = proc -> p_pid;
  *retval = 2;
  kprintf("Parent finally leaving sys_fork\n");
   return(0);
}

void uproc_thread(void *temp_tr, unsigned long k){
	(void)k;
	(void)temp_tr;
        struct addrspace *as;
        kprintf("Child - I made it to the child user uproc_thread!\n");

	//proc_remthread(curthread);      // change
        KASSERT(curproc->p_addrspace != NULL);
        as_deactivate();
        struct proc *p = curproc;
        as = curproc_setas(NULL);
        as_destroy(as);
        proc_remthread(curthread);
        proc_destroy(p);
	thread_exit();
        panic("return from thread_exit in sys_exit\n");
}
