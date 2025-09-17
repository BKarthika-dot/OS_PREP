#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#define nr 5
#define nw 5
int data=0;

pthread_mutex_t monitor_lock;
pthread_cond_t read_ok;
pthread_cond_t write_ok;

int readers=0;
int writers=0;
int waiting_writer=0;

void reader_enter(){
	pthread_mutex_lock(&monitor_lock);

	while(writers>0||waiting_writer>0){
		pthread_cond_wait(&read_ok,&monitor_lock);
	}
	readers++;
	pthread_mutex_unlock(&monitor_lock);
}

void writer_enter(){
	pthread_mutex_lock(&monitor_lock);
	waiting_writer++;
	while(readers>0||writers>0){
		pthread_cond_wait(&write_ok,&monitor_lock);
	}
	waiting_writer--;
	writers=1;
	pthread_mutex_unlock(&monitor_lock);
}

void reader_exit(){
	pthread_mutex_lock(&monitor_lock);
	readers--;
	if(readers==0){
		pthread_cond_signal(&write_ok);
	}
	pthread_mutex_unlock(&monitor_lock);
}

void writer_exit(){
	pthread_mutex_lock(&monitor_lock);
	writers=0;
	
	if(waiting_writer>0){
		pthread_cond_signal(&write_ok);
	}
	else{
		pthread_cond_broadcast(&read_ok);
	}
	pthread_mutex_unlock(&monitor_lock);
}

void* reader(void *arg){
	int id=*(int*) arg;
	for(int i=0;i<3;i++){
		reader_enter();
		int val=data;
		printf("Reader %d reading %d\n",id,val);
		usleep(100000);

		reader_exit();
		usleep(100000);
	}
	return NULL;
}
void* writer(void* arg){
	int id=*(int*) arg;
	for(int i=0;i<3;i++){
		writer_enter();
		data++;
		int val=data;
		printf("Writer %d wrote data %d\n",id,val);
		usleep(100000);
		
		writer_exit();
		usleep(100000);
	}
	return NULL;
}
int main(){
	pthread_t rthreads[nr],wthreads[nw];
	int rids[nr],wids[nw];
	
	pthread_mutex_init(&monitor_lock,NULL);
	pthread_cond_init(&read_ok,NULL);
	pthread_cond_init(&write_ok,NULL);

	for(int i=0;i<nr;i++){
		rids[i]=i;
		pthread_create(&rthreads[i],NULL,reader,&rids[i]);
	}
	for(int i=0;i<nw;i++){
		wids[i]=i;
		pthread_create(&wthreads[i],NULL,writer,&wids[i]);
	}
	
	for(int i=0;i<nr;i++){
		pthread_join(rthreads[i],NULL);
	}
	for(int i=0;i<nw;i++){
		pthread_join(wthreads[i],NULL);
	}
	
	
	pthread_mutex_destroy(&monitor_lock);
   	 pthread_cond_destroy(&read_ok);
   	 pthread_cond_destroy(&write_ok);
}

