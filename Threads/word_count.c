/*Word Count in Strings Using Threads

You have N strings (or lines).

Assign each string to one thread.

Each thread counts words in its string.

Main thread prints total words.*/
#include<stdio.h>
#include<pthread.h>
#include<string.h>

typedef struct{
    int thread_id;
    char str[100];
    int* wordNum;
}ThreadString;

void* count_words(void* arg){
    ThreadString* thread=(ThreadString*) arg;

    const char* delim=" \t\n,&?()";
    char temp[100];
    strcpy(temp,thread->str);

    int word_count=0;
    char* token=strtok(temp,delim);
    while(token!=NULL){
        word_count++;
        token=strtok(NULL,delim);
    }

    thread->wordNum[thread->thread_id]=word_count;
    printf("Number of words in Thread %d is: %d\n",thread->thread_id,word_count);
    return NULL;
}
int main(){
    int string_count;
    scanf("%d",&string_count);
    getchar();

    char string_array[string_count][100];

    pthread_t threads[string_count];   //one thread for each string
    
    ThreadString t[string_count];      //array of objects for struct

    int wordNum[string_count];

    for(int i=0;i<string_count;i++){
        fgets(string_array[i],sizeof(string_array[i]),stdin);
        string_array[i][strcspn(string_array[i],"\n")]='\0';
    }
    for(int i=0;i<string_count;i++){
        strcpy(t[i].str,string_array[i]);
        t[i].wordNum=wordNum;
        t[i].thread_id=i;
        pthread_create(&threads[i],NULL,count_words,&t[i]);
    }
    for(int i=0;i<string_count;i++){
        pthread_join(threads[i],NULL);
    }
    int total=0;
    for(int i=0;i<string_count;i++){
        total+=wordNum[i];
    }
    printf("Total Words: %d",total);
    return 0;
}