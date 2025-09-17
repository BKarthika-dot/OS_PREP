#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#define n 10

int in=0;
int out=0;
int count=0;

int buffer[n];
pthread_mutex_t monitor_lock;
pthread_cond_t notFull,notEmpty;

void insert(int item){
	pthread_mutex_lock(&monitor_lock);
	while(count==n){
		pthread_cond_wait(&notFull,&monitor_lock);
	}
	buffer[in]=item;
	in=(in+1)%n;
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
	out=(out+1)%n;
	count--;
	
	pthread_cond_signal(&notFull);
	pthread_mutex_unlock(&monitor_lock);

	return item;
}
void* producer(void* arg){
	for(int i=0;i<10;i++){
		insert(i);
		printf("Produced item %d\n",i);
		usleep(100000);
	}
	return NULL;
}
void* consumer(void* arg){
	for(int i=0;i<10;i++){
		int item=remove_item();
		printf("Consumed item %d\n",item);
		usleep(100000);
	}
	return NULL;
}
int main(){

	pthread_t prod,cons;

	pthread_mutex_init(&monitor_lock,NULL);
	pthread_cond_init(&notEmpty,NULL);
	pthread_cond_init(&notFull,NULL);
	
	pthread_create(&prod,NULL,producer,NULL);
	pthread_create(&cons,NULL,consumer,NULL);
	
	pthread_join(prod,NULL);
	pthread_join(cons,NULL);

	pthread_mutex_destroy(&monitor_lock);
	pthread_cond_destroy(&notEmpty);
	pthread_cond_destroy(&notFull);
}

