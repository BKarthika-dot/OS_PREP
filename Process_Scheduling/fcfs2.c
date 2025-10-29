#include<stdio.h>
#define max 5

typedef struct process{
    char name[50];
    int arrival_time;
    int burst_time;
    int tat;
    int completion_time;
    int wait_time;
}process;


void order_wrt_arrival_times(process* p){
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
void calculate(process* p){
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
    int sum_tat=0;
    int sum_wait=0;
    for(int i=0;i<max;i++){
        sum_tat+=p[i].tat;
        sum_wait+=p[i].wait_time;
    }
    printf(" Average tat is: %.2f\n",sum_tat/max);
    printf(" Average wait time is: %.2f\n",sum_wait/max);

}
int main(){
    process p[max];
    for(int i=0;i<max;i++){
        scanf("%s",p[i].name);
        scanf("%d",&p[i].arrival_time);
        scanf("%d",&p[i].burst_time);
        getchar();
        p[i].completion_time=-1;
        p[i].tat=-1;
        p[i].wait_time=-1;
    }
    order_wrt_arrival_times(p);
    calculate(p);

    printf("Final values for process: \n");

    printf("\n%-10s %-15s %-15s %-15s %-10s %-10s\n","Name", "Arrival", "Burst", "Completion", "TAT", "Wait");
    for(int i=0;i<max;i++){
         printf("%-10s %-15d %-15d %-15d %-10d %-10d\n",
               p[i].name, p[i].arrival_time, p[i].burst_time,
               p[i].completion_time, p[i].tat, p[i].wait_time);
    }
    return 0;
}