/*here is a study room with 10 seats shared by members of Club A and Club B.
Rules for access are:

The total number of students (A + B) cannot exceed 10 at any time.

No more than 6 students from the same club can be inside simultaneously.

Students arrive randomly from both clubs and want to study for some time (sleep to simulate).

When a student leaves, another waiting student (from either club) may enter if the above rules allow it.*/
#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
#include<stdlib.h>

#define max_seats 10
#define clubMax 6

sem_t mutex;
sem_t seats; //initialised to 10 for total no of seats
sem_t clubA_limit,clubB_limit; //both initialised to 6 to keep track of club limits


void* clubA(void* arg){
    int id=*(int*)arg;

    sem_wait(&seats);
    sem_wait(&clubA_limit);

    sem_wait(&mutex);
    printf("Student %d from Club A has entered the study room.\n", id);
    sem_post(&mutex);

    usleep(1000000+rand()%50000);

    sem_wait(&mutex);
    printf("Student %d from Club A is leaving.\n", id);
    sem_post(&mutex);

    sem_post(&seats);
    sem_post(&clubA_limit);

    pthread_exit(NULL);
}

void* clubB(void* arg){
    int id=*(int*)arg;

    sem_wait(&seats);
    sem_wait(&clubB_limit);

    sem_wait(&mutex);
    printf("Student %d from Club B has entered the study room.\n", id);
    sem_post(&mutex);

    usleep(1000000+rand()%50000);

    sem_wait(&mutex);
    printf("Student %d from Club B is leaving.\n", id);
    sem_post(&mutex);

    sem_post(&seats);
    sem_post(&clubB_limit);

    pthread_exit(NULL);
}

int main(){
    pthread_t Club_A[clubMax],Club_B[clubMax];
    int ids[clubMax];

    sem_init(&mutex,0,1);
    sem_init(&seats,0,max_seats);
    sem_init(&clubA_limit,0,clubMax);
    sem_init(&clubB_limit,0,clubMax);

    for(int i=0;i<clubMax;i++){
        ids[i]=i;
        pthread_create(&Club_A[i],NULL,clubA,&ids[i]);
        pthread_create(&Club_B[i],NULL,clubB,&ids[i]);
    }

    for(int i=0;i<clubMax;i++){
        pthread_join(Club_A[i],NULL);
        pthread_join(Club_B[i],NULL);
    }

    sem_destroy(&seats);
    sem_destroy(&clubB_limit);
    sem_destroy(&clubA_limit);
    sem_destroy(&mutex);
}