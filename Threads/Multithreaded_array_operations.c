/*
Multithreaded Array Operations using Pthreads in C

This program demonstrates the use of POSIX threads to perform multiple operations on an array:
1. Compute partial and total sums using multiple threads.
2. Separate even and odd elements concurrently.
3. Reverse the array in parallel.
4. Check if the array is a palindrome using multiple threads.

Mutex locks are used to ensure thread-safe updates to shared resources.
*/


#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#define threadcount 3


int n;
int *a;


int partial_sum[10];
int idx=0;

int odd_array[10];
int even_array[10];
int even_idx=0;
int odd_idx=0;

int isPalindrome=1;

pthread_mutex_t lock;   //mutex to prevent race condition while updating array. 2 threads may try to modify idx at the same time hence mutex lock is used

typedef struct Threads{
    int thread_id;
    int start;
    int end;
}ThreadData;


void* find_sum(void* arg){
    ThreadData* t=(ThreadData*) arg;
    int sum=0;
    for(int i=t->start;i<t->end;i++){
        sum+=a[i];
    }

    pthread_mutex_lock(&lock);
    partial_sum[idx++]=sum;
    pthread_mutex_unlock(&lock);

    printf("sum calculated for thread %d\n",t->thread_id);

    return NULL;

}

void* print_odd_even(void* arg){
    ThreadData* t=(ThreadData*)arg;

    for(int i=t->start;i<t->end;i++){
        pthread_mutex_lock(&lock);
        if(a[i]%2==0){
            even_array[even_idx++]=a[i];
        }
        else{
            odd_array[odd_idx++]=a[i];
        }
        pthread_mutex_unlock(&lock);
    }
    printf("arrays updated by thread %d\n",t->thread_id);
    return NULL;
}

void* reverse(void* arg){
    ThreadData* t=(ThreadData*)arg;
    printf("thread %d has reversed the segment from indexes [%d to %d]\n",t->thread_id,t->start,t->end);
    for(int i=t->start;i<t->end  && i<n/2;i++){
        int temp=a[i];
        a[i]=a[n-i-1];
        a[n-i-1]=temp;
    }
    return NULL;
}

void* check_palindrome(void* arg){
    ThreadData* t=(ThreadData*)arg;
    printf("checking palindrome for thread %d\n",t->thread_id);
    if(a[t->start]!=a[t->end]){
        isPalindrome=0;
    }
    return NULL;
}

int main(){
    scanf("%d",&n);
    a=(int*)malloc(n*sizeof(int));

    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    pthread_t threads[threadcount];     //array of threads
    ThreadData thread_data[threadcount];    //array of structures consisting of each threads info
    pthread_mutex_init(&lock,NULL);

    //first operation for finding sum of array
    for(int i=0;i<threadcount;i++){
        thread_data[i].thread_id=i;
        thread_data[i].start=i*(n/threadcount);
        thread_data[i].end=(i==threadcount-1)?n : (i+1)*(n/threadcount);
        pthread_create(&threads[i],NULL,find_sum,&thread_data[i]);
    }
    //terminating threads after finishing execution
    for(int i=0;i<threadcount;i++){
        pthread_join(threads[i],NULL);
    }
    int total=0;
    for(int i=0;i<threadcount;i++){
        total+=partial_sum[i];
    }
    printf("Total sum: %d\n",total);
    /*---------------------------------------------------------------------------------------------------------------------------------------*/


    //second operation for printing even and odd elements
    for(int i=0;i<threadcount;i++){
        thread_data[i].start=i*(n/threadcount);
        thread_data[i].thread_id=i;
        thread_data[i].end=(i==threadcount-1)? n:(i+1)*(n/threadcount);
        pthread_create(&threads[i],NULL,print_odd_even,&thread_data[i]);
    }
    //terminating threads after finishing execution
    for(int i=0;i<threadcount;i++){
        pthread_join(threads[i],NULL);
    }
    printf("even array:\n");
    for(int i=0;i<even_idx;i++) printf("%d ", even_array[i]);
    printf("\nodd array:\n");
    for(int i=0;i<odd_idx;i++) printf("%d ", odd_array[i]);
    printf("\n");
    /*---------------------------------------------------------------------------------------------------------------------------------------*/
    
    //third operation for reversing array
    for(int i=0;i<threadcount;i++){
        thread_data[i].start=i*(n/threadcount);
        thread_data[i].end=(i==threadcount-1)? n:(i+1)*(n/threadcount);
        thread_data[i].thread_id=i;
        pthread_create(&threads[i],NULL,reverse,&thread_data[i]);
    }
    for(int i=0;i<threadcount;i++){
        pthread_join(threads[i],NULL);
    }
    printf("reversed array:\n");
    for(int i=0;i<n;i++){
        printf("%d ",a[i]);
    }
    printf("\n");
    /*---------------------------------------------------------------------------------------------------------------------------------------*/

    //fourth operation pallindrome checking for array
    pthread_t palindrome_threads[n/2];
    ThreadData palindrome_thread_data[n/2];

    for(int i=0;i<n/2;i++){
        palindrome_thread_data[i].start=i;
        palindrome_thread_data[i].end=n-i-1;
        palindrome_thread_data[i].thread_id=i;
        pthread_create(&palindrome_threads[i],NULL,check_palindrome,&palindrome_thread_data[i]);
    }
    for(int i=0;i<n/2;i++){
        pthread_join(palindrome_threads[i],NULL);
    }
    if(isPalindrome){
        printf("given array is a palindrome\n");
    }
    else{
        printf("given array is not a palindrome\n");
    }
    return 0;
}