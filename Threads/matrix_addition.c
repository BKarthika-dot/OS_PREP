/*
Multithreaded Matrix Addition
--------------------------------
This program demonstrates the use of multiple threads to perform matrix addition efficiently.
Each thread is assigned one row of the matrices to compute in parallel, showcasing the concept
of data parallelism. The result is stored in a third matrix after all threads complete execution.
*/


#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
int n;
int *matrix1;
int *matrix2;
int *matrix_result;

typedef struct ThreadData{
    int row_number;
    int thread_id;
}ThreadData;

void* add_matrix(void* arg){

    ThreadData* t=(ThreadData*)arg;

    printf("thread %d performing addition\n",t->thread_id);

    for(int j=0;j<n;j++){
        matrix_result[t->row_number*n+j]=matrix1[t->row_number*n+j]+matrix2[t->row_number*n+j];
    }
    return NULL;
}
int main(){
    scanf("%d",&n);

    matrix1=(int*)malloc(n*n*sizeof(int));
    matrix2=(int*)malloc(n*n*sizeof(int));
    matrix_result=(int*)malloc(n*n*sizeof(int));
    printf("enter matrix 1:\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            scanf("%d",&matrix1[i*n+j]);
        }
    }

    printf("enter matrix 2:\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            scanf("%d",&matrix2[i*n+j]);
        }
    }

    ThreadData thread_data[n];
    pthread_t threads[n];

    for(int i=0;i<n;i++){
        thread_data[i].row_number=i;
        thread_data[i].thread_id=i;
        pthread_create(&threads[i],NULL,add_matrix,&thread_data[i]);
    }
    for(int i=0;i<n;i++){
        pthread_join(threads[i],NULL);
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%d ",matrix_result[i*n+j]);
        }
        printf("\n");
    }
}