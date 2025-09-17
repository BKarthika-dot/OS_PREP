#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

#define N 5         //buffer size
#define ITEMS 10    //no of items to produce

int buffer[N];
int in=0;  //index where producer will put the next item
int out=0;   //index where consumer will take next item

sem_t mutex,empty,full;

//Producer
void* producer(void* arg){
    for(int i=0;i<ITEMS;i++){
        
        sem_wait(&empty);   
        sem_wait(&mutex);

        buffer[in]=i;  //placing ITEM inside the buffer
        printf("Produced: %d\n",i);
        in=(in+1)%N;

        sem_post(&mutex);
        sem_post(&full);

        usleep(100000);    //small delay to interleave output
    }
    return NULL;
}

//Consumer 
void* consumer(void* arg){
    for(int i=0;i<ITEMS;i++){
        
        sem_wait(&full);
        sem_wait(&mutex);

        int item=buffer[out];
        printf("Consumed: %d\n",item);
        out=(out+1)%N;

        sem_post(&empty);
        sem_post(&mutex);

        usleep(100000);
    }
    return NULL;
}
int main(){
    pthread_t prod,cons;

    sem_init(&mutex,0,1);
    sem_init(&full,0,0);
    sem_init(&empty,0,N);
    
    pthread_create(&prod,NULL,producer,NULL);
    pthread_create(&cons,NULL,consumer,NULL);

    pthread_join(prod,NULL);
    pthread_join(cons,NULL);

    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}