#include <iostream>
#include <cstdlib>
#include <pthread.h>
using namespace std;
#define NUM_THREADS    4
int tot;
struct thread_data
{
    int thread_id, share_num;
}thread_datas[NUM_THREADS];
void *service_thread_func(void* arg)
{
    struct thread_data * data = (struct thread_data*)arg;
    for (int i = 0; i < 1000000; ++i)
    {
//        data->share_num++;
        tot++;
    }
    printf("thread_id = %d\t share_num=%d\n", data->thread_id, tot);
    return NULL;
}
int main ()
{
    pthread_t *tids = NULL;
    tids = (pthread_t *)malloc(sizeof(pthread_t) * NUM_THREADS); //整个线程池启4个多线程
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        thread_datas[i].thread_id = i;
    }
    int ret;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        ret = pthread_create(&tids[i], NULL, service_thread_func, (void *)&thread_datas[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; ++i){
        printf("Start %d\n", i);
        pthread_join(tids[i],NULL);
    }
    
    return 0;
}
