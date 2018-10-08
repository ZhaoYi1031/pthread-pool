#include <stdint.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

sem_t sem;

void *testfunc(void *arg)
{
    while(1)
    {
        sem_wait(&sem);
        //do something....
        printf("hello world...\n");
    }
}

int main()
{
    pthread_t ps;
    sem_init(&sem, 0, 0);
    
    sem_post(&sem);
    printf("sem=%d\n", sem);
    sem_post(&sem);
    printf("sem=%d\n", sem);
    sem_post(&sem);
    printf("sem=%d\n", sem);
    sem_post(&sem);
    printf("sem=%d\n", sem);
    
    sem_wait(&sem);
    printf("sem=%d\n", sem);
    sem_wait(&sem);
    printf("sem=%d\n", sem);
    sem_wait(&sem);
    printf("sem=%d\n", sem);
    sem_wait(&sem);
    printf("sem=%d\n", sem);
    
    return 0;
    
    pthread_create(&ps,NULL,testfunc,NULL);
    while(1)
    {
        //每隔一秒sem_post 信号量sem加1 子线程sem_wait解除等待 打印hello world
        sem_post(&sem);
//        printf("--CNM\n");
        sleep(1);
        printf("++CNM\n");
    }
    return 0;
}
