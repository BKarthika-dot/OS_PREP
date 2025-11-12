/*🧩 Problem 1 — “Priority Producer–Consumer using Monitors”

🧠 Scenario:
You’re managing a print job server.
There are 2 types of producers:
Normal users
Admin users (priority producers)
Both produce print jobs that go into a shared buffer.
Consumers are printers that remove and print jobs.

⚙️ Constraints:
The buffer has a fixed size (say, 5 jobs max).
Admin jobs must always be printed before any normal jobs, if both exist.
Use monitors (a pthread_mutex_t + condition variables) for synchronization.
Avoid starvation — normal users should still get printed when no admin jobs remain.*/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#define N 5

pthread_mutex_t monitor_lock;
pthread_cond_t notFull,notEmpty;

int buffer[N];
int in=0,out=0,count=0;
int admin_active=0;

pthread_cond_t admin_using;

void insert_item(int item,int isAdmin){

    pthread_mutex_lock(&monitor_lock);

    while(count==N  || (isAdmin==0 && admin_active==1)){
        if(count==N){
            pthread_cond_wait(&notFull,&monitor_lock);
        }
        else{
            pthread_cond_wait(&admin_using,&monitor_lock);    //regular user waits until admin stops using 
        }
    }

    if(isAdmin){
        admin_active=1; //admin starts producing
    }
    buffer[in]=item;
    in=(in+1)%N;
    count++;

    pthread_cond_signal(&notEmpty);
    pthread_mutex_unlock(&monitor_lock);

}
int remove_item(){

    pthread_mutex_lock(&monitor_lock);

    while(count==0){
        pthread_cond_wait(&notEmpty,&monitor_lock);
    }

    int item=buffer[out];
    out=(out+1)%N;
    count--;

    // If buffer empty, reset admin flag
    if (count == 0) {
        admin_active = 0;
        pthread_cond_broadcast(&admin_using);
    }

    pthread_cond_signal(&notFull);
    pthread_mutex_unlock(&monitor_lock);

    return item;
}

void* consumer(void* arg){
    for(int i=0;i<N;i++){
        int item=remove_item();
        printf("Consumer has consumed %d\n",item);
        usleep(1000000);
    }
    return NULL;
}

void* admin_producer(void* arg){

    for(int i=0;i<N;i++){
        insert_item(100+i,1);
        printf("Admin  has produced %d\n",100+i);
        usleep(1000000);
    }

    pthread_mutex_lock(&monitor_lock);
    admin_active = 0;
    pthread_cond_broadcast(&admin_using); // Admin is done — all regular producers waiting can now re-check and possibly start producing
    pthread_mutex_unlock(&monitor_lock);

    return NULL;
}

void* regular_producer(void* arg){
    
    for(int i=0;i<N;i++){
        insert_item(i,0);
        printf("Regular User has produced %d\n",i);
        usleep(1000000);
    }
    return NULL;
}
int main(){
    pthread_t admin,regular,cons;

    pthread_mutex_init(&monitor_lock,NULL);
    pthread_cond_init(&notFull,NULL);
    pthread_cond_init(&notEmpty,NULL);
    pthread_cond_init(&admin_using,NULL);

    pthread_create(&admin,NULL,admin_producer,NULL);
    pthread_create(&regular,NULL,regular_producer,NULL);
    pthread_create(&cons,NULL,consumer,NULL);

    pthread_join(admin,NULL);
    pthread_join(regular,NULL);
    pthread_join(cons,NULL);

    pthread_mutex_destroy(&monitor_lock);
    pthread_cond_destroy(&notEmpty);
    pthread_cond_destroy(&notFull);
    pthread_cond_destroy(&admin_using);
}

