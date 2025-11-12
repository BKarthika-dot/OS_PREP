/*This program simulates a shared printer scenario in an office where multiple employees (threads) compete to use two printers. It uses POSIX threads (pthreads) and semaphores to handle synchronization.

Each employee thread randomly decides which printer to try first:
If the chosen printer is available, it proceeds to print.
If it’s busy, it tries the second printer.
If both printers are occupied, the employee waits until one becomes free.
The use of sem_trywait() ensures non-blocking attempts to acquire a printer, while sem_wait() handles waiting when both are busy.
This creates a realistic simulation of resource sharing and scheduling among concurrent processes.*/

#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define employee_count 10

sem_t mutex1,mutex2;

void* choose_and_print(void* arg){

    int id=*(int*)arg;
    int first=rand()%2; //to randomly choose first printer to be used

    sem_t *first_mutex=(first==1)? &mutex1:&mutex2;
    sem_t *second_mutex=(first==1)? &mutex2:&mutex1;

    if(sem_trywait(first_mutex)==0){
        printf("Printer 1 printing for employee %d\n",id);
        usleep(1000000+rand()%500000);
        sem_post(first_mutex);
    }
    else if(sem_trywait(second_mutex)==0){
        printf("Printer 2 printing for employee %d\n",id);
        usleep(1000000+rand()%500000);
        sem_post(second_mutex);
    }
    else {
        printf("Both printers busy, employee %d is waiting...\n", id);
        sem_wait(first_mutex); // wait for printer 1 to be free
        printf("Employee %d now using printer 1 after waiting.\n", id);
        usleep(1000000 + rand() % 500000);
        sem_post(first_mutex);
    }
    return NULL;
}

int main(){
    pthread_t employees[employee_count];
    int ids[employee_count];

    sem_init(&mutex1,0,1);
    sem_init(&mutex2,0,1);
    

    for(int i=0;i<employee_count;i++){
        ids[i]=i;
        pthread_create(&employees[i],NULL,choose_and_print,&ids[i]);
        usleep(1000000); //time delay to simulate staggerred print requests
    }

    for(int i=0;i<employee_count;i++){
        pthread_join(employees[i],NULL);
    }

    sem_destroy(&mutex1);
    sem_destroy(&mutex2);
}

