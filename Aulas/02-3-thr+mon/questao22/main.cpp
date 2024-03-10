#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <libgen.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "thread.h"
int share;
void  *thread_main(void* arg){
    int N2 = 0;
    printf("[thread_child] START!\n");
    printf("Enter a value between 10 and 20: ");
    scanf("%d",&N2);

    for(;share<=N2;share++){
        printf("%2d\n",share);
    }
    printf("[thread_child] END!\n");
    return NULL;
}

int main(){
    pthread_t a_thread;
    int N1;
    N1=0;
    printf("Enter a value between 1 and 9: ");
    scanf("%d",&N1);
    printf("%d\n",N1);
    share = N1;
    printf("[thread_main] START!\n");
    pthread_create(&a_thread, NULL, thread_main, NULL);
    pthread_join(a_thread, NULL);
    while(share-->1){
        printf("%2d\n",share);
    }
    printf("[thread_main] END!\n");
    return 0;

}