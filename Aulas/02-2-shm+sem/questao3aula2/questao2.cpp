#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "process.h"



int main(void){

    int shmid = pshmget(IPC_PRIVATE,sizeof(int),0600 | IPC_CREAT);
    int *pcounter = (int *)pshmat(shmid,NULL,0);
    int sem = psemget(IPC_PRIVATE, 2, 0600 | IPC_CREAT);

    int num;
    do{
        printf("Insira um nยบ entre 10 e 20: ");
        scanf("%d",&num);
        if(num<10 || num >20)
            printf("Invalid input");
        printf("\n");

    }while(num<10 || num >20);
    *pcounter=num;
    
    int ret = pfork();
    
    if (ret == 0){  // se for o filho
        int ret2 = pfork();
        if(*pcounter%2 == 1){
            psem_up(sem,0);                
        }
        else{
            psem_up(sem,1);
        }
        while(*pcounter>2){    
            if(ret2==0){
                psem_down(sem,0);
                if(*pcounter%2 == 1){   
                    (*pcounter)--;
                    printf("(%d) pcounter = %d\n",getpid(),*pcounter);
                }
                psem_up(sem,1);
            }
            else{
                psem_down(sem,1);
                if(*pcounter%2 == 0){   
                    (*pcounter)--;
                    printf("(%d) pcounter = %d\n",getpid(),*pcounter);
                }
                psem_up(sem,0);
            }
        }

        
    }
    else{
        pwait(NULL);
        psemctl(sem,2,IPC_RMID);

    }
    return 1;
}