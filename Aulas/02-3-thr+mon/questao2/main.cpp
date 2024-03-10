#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "thread.h"

void *thread_main(void *arg)
{
    printf("[thread] START!\n");
    usleep(500000);
    printf("[thread] END!\n");
    
}

int main()
{
    pthread_t a_thread;
    printf("[main] START!\n");
    pthread_create(&a_thread, NULL, thread_main, NULL);
    pthread_join(a_thread, NULL);
    printf("[main] END!\n");
    return 0;
}
