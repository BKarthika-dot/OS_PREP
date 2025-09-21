/*First Come First Serve algorithm for process scheduling
Goal is to calculate the completion time, avg turn around time and avg waiting time for each process*/
#include<stdio.h>
#define max 5 //no of process

typedef struct {
    char name[50];
    int arrival_time;
    int burst_time;
    int completion_time;
    int tat;
    int wait_time;
}process;

process cq[max]  ;   //declaring a circular queue

int front=-1;
int rear=-1;

int isEmpty(){
    return front==-1;
}
int isFull(){
    return (front==(rear+1)%max);
}
void enqueue(process p){
    if(isFull()){
        printf("queue is full\n");
        return;
    }
    if(isEmpty()){
        front=rear=0;
        cq[rear]=p;
    }
    else{
        rear=(rear+1)%max;
        cq[rear]=p;
    }
}
void dequeue(){
    process p;
    if(isEmpty()){
        printf("queue is empty\n");
        return;
    }
    p=cq[front];
    printf("%s %d %d %d %d %d \n",p.name,p.arrival_time,p.burst_time,p.completion_time,p.tat,p.wait_time);
    if(front==rear){
        front=rear=-1;
    }
    else{
        front=(front+1)%max;
    }
}
//sorting the processes in order of their arrival times
void order(process* p){
    for(int i=0;i<max-1;i++){
        for(int j=0;j<max-i-1;j++){
            if(p[j].arrival_time>p[j+1].arrival_time){
                process temp=p[j];
                p[j]=p[j+1];
                p[j+1]=temp;
            }
        }
    }
}
void calculateValues(process* p){
    int time=-1;
    for(int i=0;i<max;i++){
        if(p[i].arrival_time>time){
            p[i].completion_time=p[i].arrival_time+p[i].burst_time;
        }
        else{
            p[i].completion_time=time+p[i].burst_time;
        }
        time=p[i].completion_time;
        p[i].tat=p[i].completion_time-p[i].arrival_time;
        p[i].wait_time=p[i].tat-p[i].burst_time;
    }

}
int main(){
    process p[max];
    for(int i=0;i<max;i++){
        scanf("%s",p[i].name);
        scanf("%d",&p[i].arrival_time);
        scanf("%d",&p[i].burst_time);
        p[i].completion_time=-1;
        p[i].tat=-1;
        p[i].wait_time=-1;
    }

    order(p);   //ordering the queue based on arrival times

    calculateValues(p);   //calculating values

    int sum_tat=0;
    int sum_wt=0;

    //finding average tat and wait time
    for(int i=0;i<max;i++){
        sum_tat+=p[i].tat;
        sum_wt+=p[i].wait_time;
    }

    //enqueuing the ordered processes into the queue
    for(int i=0;i<max;i++){
        enqueue(p[i]);  //ensure that enqueuing happens after the calculation of all values so that the updated values for completion time,tat,etc are enqueued correctly
    }

    while(!isEmpty()){
        dequeue();
    }
    printf("Average Turn Around Time: %.2f\n",(double)sum_tat/max);
    printf("Average Wait Time: %.2f\n",(double)sum_wt/max);
}