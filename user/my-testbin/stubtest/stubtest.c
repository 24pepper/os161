#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(){
        int fork_pid = (int)fork();
        printf("pid from fork is %d\n", fork_pid);
//        pid_t pid;

//        printf("Test call for fork stub\n");
//        pid = fork();
//        printf("pid: %d\n", pid);
//        return 0; /* avoid compiler warnings */
}

