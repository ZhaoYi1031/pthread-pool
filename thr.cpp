/*
 * 1p.c
 *
 *  Created on: Jan 18, 2017
 *      Author: s
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <pthread.h>

#include <unistd.h>

unsigned count = 0;
pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

void* decrement_count() {
    pthread_mutex_lock (&count_lock);
    sleep(1);
    //while(count==0)
    pthread_cond_wait( &cond, &count_lock);

    count=count -1;
    pthread_mutex_unlock (&count_lock);
    printf("decrement done,count = %d\n",count);
    return ((void *)0);
}

void increment_count(){
    pthread_mutex_lock(&count_lock);

    if(count==0)
        // pthread_cond_signal(&cond);

        count=count+1;
    pthread_mutex_unlock(&count_lock);
    printf("increment done.count = %d\n",count);
}
void test()
{
    sleep(5);
    pthread_mutex_lock(&count_lock);

    printf("sleep 5s \n");
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&count_lock);
}

int main()
{


    pthread_t idone,idtwo,id3;
    int ret;
    ret=pthread_create(&idone,NULL, &decrement_count,NULL);
    if(ret!=0){
        printf ("Create pthread 1 error!/n");
        exit (1);
    }

    ret=pthread_create(&idtwo,NULL,(void *) increment_count,NULL);
    if(ret!=0){
        printf ("Create pthread 2 error!/n");
        exit (1);
    }

    ret=pthread_create(&id3,NULL,(void *) test,NULL);
    if(ret!=0){
        printf ("Create pthread 3 error!/n");
        exit (1);
    }

    printf("count = %d\n",count);
    printf(" exec: pthread_join(idone,NULL); \n ");
    pthread_join(idone,NULL);
    pthread_join(idtwo,NULL);
    pthread_join(id3,NULL);

    printf("done\n");
    return 0;
}
