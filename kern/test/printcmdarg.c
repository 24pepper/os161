#include <types.h>
#include <lib.h>
#include <test.h>


int printcmdarg(int nargs, char **arg){
	if(nargs == 1){
                kprintf("No command line arguments were given\n");
        }
	else{
		kprintf("Number of arguments is %d, the arguments are ", nargs - 1);
	}
	if(nargs == 2){
                kprintf("%s\n", arg[0]);
	}
	else{
		for(int i = 1; i < nargs; ++i){
			if(i == nargs - 1){
				kprintf("and %s\n", arg[i]);
			}
			else{
				kprintf("%s, ", arg[i]);
			}
		}
	}
	return 0;

}

