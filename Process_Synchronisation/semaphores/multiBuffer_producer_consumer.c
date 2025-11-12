/*Scenario

You have 2 producers and 2 consumers, but unlike the standard setup,
there are 2 separate bounded buffers, bufferA and bufferB, each of size 5.

Producer P1 can produce only even-numbered items (2, 4, 6, …) → goes into bufferA.

Producer P2 can produce only odd-numbered items (1, 3, 5, …) → goes into bufferB.

Consumer C1 consumes from both buffers alternately: first from bufferA, then bufferB, then bufferA, and so on.

Consumer C2 consumes only from whichever buffer has more elements currently (i.e., if one buffer is more filled, C2 helps clear it).*/


#include<stdio.h>
#include<semaphore.h>
#include<unistd.h>
#include<pthread.h>
#define max 5
#define items 10

int buffer1[max],buffer2[max];
int buffer1_in=0,buffer1_out=0;
int buffer2_in=0,buffer2_out=0;



sem_t empty1,mutex1,full1;
sem_t empty2 , mutex2, full2;

int done1=0;  //adding these checkers to prevent consumers from infinitely waiting on full after buffers are empty
int done2=0;

void* producer1(void* arg){
    for(int i=0;i<items;i++){
        if(i%2==0){
            sem_wait(&empty1);
            sem_wait(&mutex1);

            buffer1[buffer1_in]=i;
            printf("Producer1 has placed %d inside Buffer1\n",i);
            buffer1_in=(buffer1_in+1)%max;

            sem_post(&full1);
            sem_post(&mutex1);

            usleep(1000000);
        }
    }
    done1=1; //producer 1 completed
    pthread_exit(NULL);

}

void* producer2(void* arg){
    for(int i=0;i<items;i++){
        if(i%2!=0){
            sem_wait(&empty2);
            sem_wait(&mutex2);

            buffer2[buffer2_in]=i;
            printf("Producer2 has placed %d inside Buffer2\n",i);
            buffer2_in=(buffer2_in+1)%max;

            sem_post(&full2);
            sem_post(&mutex2);

            usleep(1000000);
        }
    }
    done2=1; //producer 2 completed
    pthread_exit(NULL);
}

void* consumer1(void* arg){
    for(int i=0;i<items  ;i++){
        if(i%2==0){
            sem_wait(&full1);
            sem_wait(&mutex1);
            int item=buffer1[buffer1_out];
            printf("Consumer1 consumed %d from Buffer1\n",item);
            buffer1_out=(buffer1_out+1)%max;
            sem_post(&mutex1);
            sem_post(&empty1);
            usleep(1000000);
        }
        else{
            sem_wait(&full2);
            sem_wait(&mutex2);
            int item=buffer2[buffer2_out];
            printf("Consumer1 consumed %d from Buffer2\n",item);
            buffer2_out=(buffer2_out+1)%max;
            sem_post(&mutex2);
            sem_post(&empty2);
            usleep(1000000);
        }
    }
   pthread_exit(NULL);
}

void* consumer2(void* arg){
    for(int i=0;i<items;i++){
        int chosen=-1;
        if(done1 && done2){
            sem_wait(&mutex1);
            sem_wait(&mutex2);


            int count1 = (buffer1_in - buffer1_out + max) % max;
            int count2 = (buffer2_in - buffer2_out + max) % max;
            if (count1 >= count2 && count1 > 0) chosen = 1;
            else if (count2 > 0) chosen = 2;

            sem_post(&mutex1);
            sem_post(&mutex2);

            if (count1 == 0 && count2 == 0) {
                printf("Both buffers empty and producers finished — Consumer exiting\n");
                pthread_exit(NULL);
            }
        }

        if(chosen==-1)break;

        if(chosen==1){
            sem_wait(&full1);
            sem_wait(&mutex1);
            int item=buffer1[buffer1_out];
            printf("Consumer2 consumed %d from Buffer1\n",item);
            buffer1_out=(buffer1_out+1)%max;
            sem_post(&mutex1);
            sem_post(&empty1);
        }
        else if(chosen==2){
            sem_wait(&full2);
            sem_wait(&mutex2);
            int item=buffer2[buffer2_out];
            printf("Consumer2 consumed %d from Buffer2\n",item);
            buffer2_out=(buffer2_out+1)%max;
            sem_post(&mutex2);
            sem_post(&empty2);
        }
        usleep(1000000);
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t prod1,cons1,prod2,cons2;

    sem_init(&mutex1,0,1);
    sem_init(&empty1,0,5);
    sem_init(&full1,0,0);

    sem_init(&mutex2,0,1);
    sem_init(&empty2,0,5);
    sem_init(&full2,0,0);

    pthread_create(&prod1,NULL,producer1,NULL);
    pthread_create(&prod2,NULL,producer2,NULL);
    pthread_create(&cons1,NULL,consumer1,NULL);
    pthread_create(&cons2,NULL,consumer2,NULL);

    pthread_join(prod1,NULL);
    pthread_join(prod2,NULL);
    pthread_join(cons1,NULL);
    pthread_join(cons2,NULL);

    sem_destroy(&mutex1);
    sem_destroy(&full1);
    sem_destroy(&empty1);
    sem_destroy(&mutex2);
    sem_destroy(&full2);
    sem_destroy(&empty2);

}