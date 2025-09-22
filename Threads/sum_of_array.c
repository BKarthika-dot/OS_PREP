/*Sum of Array Using Threads

Split an array into N parts, each thread sums one part, main thread collects total.

Learn: arguments to threads, joining results.*/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

typedef struct{
    int thread_id;
    int start;
    int end;
    int* a;
    int* partial_sums;
}ThreadData;

void* findSum(void* arg){
    ThreadData* data=(ThreadData*) arg;

    int sum=0;

    for(int i=data->start;i<data->end;i++){
        sum+=data->a[i];
    }
    data->partial_sums[data->thread_id]=sum;
    printf("Partial sum of Thread %d is %d\n",data->thread_id,sum);
    return NULL;
}
int main(){
    int array_size;
    scanf("%d",&array_size);

    int a[array_size];   //declaring array

    for(int i=0;i<array_size;i++){
        scanf("%d",&a[i]);
    }

    int n;              //declaring thread count
    scanf("%d",&n);
    pthread_t threads[n];   //array of threads

    int partial_sums[n];  //declaring array for storing each sum value

    ThreadData t[n];   //declaring array of struct objects;

    int group=array_size/n;

    for(int i=0;i<n;i++){
        t[i].thread_id=i;
        t[i].start=i*group;
        t[i].end= (i==n-1) ? array_size : (i+1)*group;                              //ternary operator: (condition) ? value_if_true : value_if_false;
        t[i].a=a;
        t[i].partial_sums=partial_sums;
        pthread_create(&threads[i],NULL,findSum,&t[i]);
    }
    for(int i=0;i<n;i++){
        pthread_join(threads[i],NULL);
    }
    int total=0;
    for(int i=0;i<n;i++){
        total+=partial_sums[i];
    }
    return 0;
}