#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "delays.h"
#include "process.h"


int main(void)
{
    printf("Before the fork: PID = %d, PPID = %d\n", getpid(), getppid());

    pid_t ret=pfork();

    if (ret ==0)
    {
        for (int i=1; i<10; i++)
        {
            printf("%d\n",i );
        
        }
        printf("I'm the child: PID = %d, PPID = %d\n", getpid(), getppid());
        bwRandomDelay(0, 100000);
    }else{
        pwait(NULL);
        for(int i=11; i<20; i++)
        {
            printf("%d\n",i);
        }
        printf("I'm the parent: PID = %d, PPID = %d\n", getpid(), getppid());
        bwRandomDelay(0, 100000);
        
    }

    return EXIT_SUCCESS;
}