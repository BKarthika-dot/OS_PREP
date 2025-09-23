/*Count Even and Odd Numbers

Divide an array into N parts.

Each thread counts how many even and odd numbers are in its section.

Main thread adds up totals.*/

#include<stdio.h>
#include<pthread.h>

typedef struct{
    int thread_id;
    int start;
    int end;
    int* a;
    int* count_even;
    int* count_odd;
}Threads;

void* count_odd_even(void* arg){
    Threads* thread=(Threads*)arg;
    int odd=0;
    int even=0;
    for(int i=thread->start;i<thread->end;i++){
        if(thread->a[i]%2==0){
            even++;
        }
        else{
            odd++;
        }
    }
    thread->count_even[thread->thread_id]=even;     //storing the counts of each thread in their respective arrays as per thread_id
    thread->count_odd[thread->thread_id]=odd;
    printf("For Thread %d: Even count=%d  and  Odd count=%d\n",thread->thread_id,even,odd);
    return NULL;
}
int main(){
    int array_size;
    scanf("%d",&array_size);

    int a[array_size];
    for(int i=0;i<array_size;i++){
        scanf("%d",&a[i]);
    }

    int n;
    scanf("%d",&n);

    Threads t[n];    //declaring array of struct objects

    pthread_t threads[n];

    int count_even[n],count_odd[n];

    for(int i=0;i<n;i++){
        t[i].start=i*(array_size/n);
        t[i].end=  (i==n-1)?array_size:(i+1)*(array_size/n);
        t[i].thread_id=i;
        t[i].a=a;
        t[i].count_even=count_even;
        t[i].count_odd=count_odd;
        pthread_create(&threads[i],NULL,count_odd_even,&t[i]);
    }
    for(int i=0;i<n;i++){
        pthread_join(threads[i],NULL);
    }

    int even_total=0;
    int odd_total=0;
    for(int i=0;i<n;i++){
        even_total+=count_even[i];
        odd_total+=count_odd[i];
    }
    printf("Total even numbers=%d\n",even_total);
    printf("Total odd numbers=%d\n",odd_total);
    return 0;
}