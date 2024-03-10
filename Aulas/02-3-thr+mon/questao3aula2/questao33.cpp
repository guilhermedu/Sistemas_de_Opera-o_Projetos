#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //incluir o usleep

#include "thread.h"

//o valor do contador chega a 0
int counter;
pthread_mutex_t mtx;
pthread_cond_t cond;

void* thread_main1(void *arg)
{
    while(counter>1)
    {
        mutex_lock(&mtx);
        //print("1");
        while(counter%2 ==0)
        {
            cond_wait(&cond,&mtx);

        }
        printf("(%d) %d\n",gettid(),counter);
        counter--;
        usleep(50000);

        cond_signal(&cond);
        mutex_unlock(&mtx);

    }
    return NULL;
}

void* thread_main2(void *arg)
{
    while(counter>1)
    {
        mutex_lock(&mtx);
        //printf("2");

        while(counter%2==1)
        {
            cond_wait(&cond,&mtx);
        }

        printf("(%d) %d\n",gettid(),counter);
        counter--;

        usleep(50000);
        cond_signal(&cond);
        mutex_unlock(&mtx);

    }
    return NULL;
}

int main()
{
    int read;
    do{
        printf("Insira um nº entre 10 e 20:");
        scanf("%d",&read);
        if(read<10 || read>20)
        {
            printf("invalid input");
        }
        printf("\n");

    }while(read<10 || read >20);

    counter=read;
    pthread_t a_thread;
    pthread_t b_thread;
    mutex_init(&mtx,NULL);
    //cond_init(&cond,NULL); // pass the address of the variable to cond_init
    thread_create(&a_thread,NULL,thread_main1,NULL); // cria a thread e vai para a função "thread_main"
    thread_create(&b_thread,NULL,thread_main2,NULL);
    cond_signal(&cond); // Signal thread_main1 to start

    thread_join(a_thread,NULL); // espera que o "thread_main1" acabe para iniciar
    thread_join(b_thread,NULL); // espera que o "thread_main2" acabe para iniciar
    printf("(parent) DONE!!\n");
    return 0;

}