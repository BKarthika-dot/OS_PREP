/*Hello from Threads

Create 5 threads. Each thread just prints "Hello from thread X".

Learn: pthread_create, pthread_join, passing IDs.*/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#define max 5

void* print_hello(void* arg){
    int id=*(int*) arg;
    printf("Hello World from Thread %d\n",id);
    return NULL;
}
int main(){
    pthread_t threads[max];
    int ids[max];

    for(int i=0;i<max;i++){
        ids[i]=i+1;
        pthread_create(&threads[i],NULL,print_hello,&ids[i]);
    }
    for(int i=0;i<max;i++){
        pthread_join(threads[i],NULL);
    }
    return 0;
}