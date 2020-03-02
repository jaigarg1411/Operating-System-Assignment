#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"
#include <stdbool.h>
#include <unistd.h>

sem_t full, empty;
buffer_item BUFFER[BUFFER_SIZE]; //buffer
pthread_t tid;
pthread_attr_t attr;
pthread_mutex_t mutex;
int buffer_index;


int remove_item(buffer_item* item)
{
    if(buffer_index > 0)
    {
        *item = BUFFER[buffer_index-1];
        buffer_index--;
        return 0;
    }

    return -1;
}

void* consumer(void* param)
{
    while(true)
    {
        int rand_sleep_cons = rand()%4;
        sleep(rand_sleep_cons);

        buffer_item item_removed;

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if(remove_item(&item_removed))
            printf("Consumer ERROR\n");

        else
            printf("Consumer consumed item: %d\n",item_removed);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int insert_item(buffer_item item)
{
    if(buffer_index < BUFFER_SIZE)
    {
        BUFFER[buffer_index++] = item;
        return 0;
    }

    return -1;
}

void* producer(void* param)
{
    while(true)
    {
        int rand_sleep_prod = rand()%4;
        sleep(rand_sleep_prod);

        buffer_item item_to_be_inserted = rand();

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        if(insert_item(item_to_be_inserted))
            printf("Producer ERROR\n");

        else
            printf("Producer produced item: %d\n",item_to_be_inserted);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        printf("Too few arguments passed.....EXIT program\n");
        exit(0);
    }

    if(argc > 4)
    {
        printf("Too many arguments passed.....EXIT program\n");
        exit(0);
    }

    int sleep_time, prod_threads, cons_threads;
    sleep_time = atoi(argv[1]);
    prod_threads = atoi(argv[2]);
    cons_threads = atoi(argv[3]);

    if(sleep_time < 0 || prod_threads < 0 || cons_threads < 0)
    {
        printf("Only positive arguments allowed.....EXIT program\n");
        exit(0);
    }

    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    pthread_attr_init(&attr);
    buffer_index = 0;

    int i;
    for(i=0;i<prod_threads;i++)
        pthread_create(&tid,&attr,producer,NULL);

    for(i=0;i<cons_threads;i++)
        pthread_create(&tid,&attr,consumer,NULL);


    sleep(sleep_time);
    printf("Exiting program after sleep\n");
    exit(0);
}