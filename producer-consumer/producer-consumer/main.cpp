#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <semaphore.h>
#include<unistd.h>

/* 将信号量定义为全局变量，方便多个线程共享 */
sem_t sem;

/* 线程1和线程2的公用命令 */
int gCmd = 0;

/* 同步线程1和线程2的全局变量 */
static int gIsExecFlag = 0;

/* 定义线程pthread1 */
static void * pthread1(void *arg)
{
    /* 线程pthread1开始运行 */
    printf("pthread1 start!\n");
    
    while(1)
    {
        /* 等待没有命令正在执行 */
        while(gIsExecFlag);
        
        printf("-------------------------\n");
        /* 更新命令 */
        gCmd++;
        if(gCmd == 10)
        {
            /* 释放信号量 */
            sem_post(&sem);
            printf("**************************\n");
            /* 发送命令结束 */
            return NULL;
        }
        
        printf("---sem=%d\n", sem);
        /* 释放信号量 */
        sem_post(&sem);
        printf("+++sem=%d\n", sem);
        
        /* 等待线程2执行命令 */
        sleep(1);
    }
}

/* 定义线程pthread2 */
static void * pthread2(void *arg)
{
    int tmp;
    
    /* 线程pthread2开始运行 */
    printf("pthread2 start!\n");
    
    while(1)
    {
        if (sem_wait(&sem) != 0)
        {
            printf("Error!\n");
        }
        
        
        
        /* 正在执行的标志置1 */
        gIsExecFlag = 1;
        
        /* 线程2接受来自线程1的命令，并打印 */
        tmp = gCmd;
        printf("now execute the cmd,and the code of cmd is %d.\n", tmp);
        
        /* 执行命令需要时间：3s，模仿实际命令执行 */
        sleep(3);
        
        /* 正在执行的标志清0 */
        gIsExecFlag = 0;
        
        if(gCmd == 10){
            /* 命令执行结束 */
            return NULL;
        }
    }
}

/* main函数 */
int main(int agrc,char* argv[])
{
    pthread_t tidp1,tidp2;
    
    /* 初始化信号量sem,注意初始值为0 */
    sem_init(&sem, 0, 0);
    
    /* 创建线程pthread1 */
    if ((pthread_create(&tidp1, NULL, pthread1, NULL)) == -1)
    {
        printf("create error!\n");
        return 1;
    }
    
    /* 同步，让线程1先执行 */
    usleep(10);
    
    /* 创建线程pthread2 */
    if ((pthread_create(&tidp2, NULL, pthread2, NULL)) == -1)
    {
        printf("create error!\n");
        return 1;
    }
    
    /* 等待线程pthread1释放 */
    if (pthread_join(tidp1, NULL))
    {
        printf("thread is not exit...\n");
        return -2;
    }
    
    /* 等待线程pthread2释放 */
    if (pthread_join(tidp2, NULL))
    {
        printf("thread is not exit...\n");
        return -2;
    }
    
    return 0;
}
