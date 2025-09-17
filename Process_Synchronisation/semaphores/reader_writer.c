#include<stdio.h>
#include<semaphore.h>
#include<unistd.h>
#include<pthread.h>
#define SIZE 5

sem_t mutex,rw_mutex;
int readcount=0;
int data=0;

void* writer(void* arg){
    int id = *(int*)arg; //thread id
    for(int i=0;i<SIZE;i++){
        sem_wait(&rw_mutex);

        data++;

        printf("Writer %d: Writing data %d\n",id,data);

        sem_post(&rw_mutex);
        usleep(100000);
    }
    return NULL;
}
void* reader(void* arg){
    int id = *(int*)arg;    //thread id
    for(int i=0;i<SIZE;i++){
        sem_wait(&mutex);
        readcount++;
        if(readcount==1){
            sem_wait(&rw_mutex);
        }
        sem_post(&mutex);

        printf("Reader %d: Reading data %d\n",id,data);

        sem_wait(&mutex);
        readcount--;

        if(readcount==0){
            sem_post(&rw_mutex);
        }
        sem_post(&mutex);

        usleep(100000);
    }
    return NULL;
}
int main(){
    pthread_t r[SIZE],w[SIZE];
    int ids[SIZE];
    sem_init(&mutex,0,1);
    sem_init(&rw_mutex,0,1);

    for(int i=0;i<SIZE;i++){
        ids[i]=i+1;
        pthread_create(&r[i],NULL,reader,&ids[i]);
        pthread_create(&w[i],NULL,writer,&ids[i]);
    }
    for(int i=0;i<SIZE;i++){
        pthread_join(r[i],NULL);
        pthread_join(w[i],NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&rw_mutex);
}