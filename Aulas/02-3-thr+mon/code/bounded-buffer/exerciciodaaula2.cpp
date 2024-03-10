#include <stdio.h>
#include <stdlib.h>

#include "thread.h"

void *thread_main(void *arg)
{
    printf("[thread] START!\n");
    thread_exit(NULL);
    printf("[thread] END!\n");
    return NULL;
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

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
{
    require (t != NULL, "pthread_t target variable required");
   require (thread_main != NULL, "thread main function required");

   int status = pthread_create(t, attr, thread_main, arg);
   check_error(status);
}