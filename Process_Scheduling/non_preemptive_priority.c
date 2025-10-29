#include<stdio.h>
#define max 5

typedef struct process{
    char name[50];
    int arrival_time;
    int burst_time;
    int tat;
    int completion_time;
    int wait_time;
    int priority;
    int done;
}process;
void order(process *p){
    for(int i=0;i<max-1;i++){
        for(int j=0;j<max-i-1;j++){
            if(p[i].arrival_time>p[i+1].arrival_time){
                process temp=p[i];
                p[i]=p[i+1];
                p[i+1]=temp;
            }
        }
    }
}
void calculate(process *p){
    int time=-1;
    int sum_tat=0,sum_wait=0;
    int completed=0;

    while(completed<max){
        int idx=-1;
        int highest_priority=99999;

        for(int i=0;i<max;i++){
            if(p[i].arrival_time<=time&&p[i].done==0){
                if(p[i].priority<highest_priority){
                    highest_priority=p[i].burst_time;
                    idx=i;
                }
            }
        }
        if(idx==-1){
            time++;
            continue;
        }
         // execute process
        time += p[idx].burst_time;
        p[idx].completion_time = time;
        p[idx].tat = p[idx].completion_time - p[idx].arrival_time;
        p[idx].wait_time = p[idx].tat - p[idx].burst_time;
        p[idx].done = 1;
        completed++;

        sum_tat += p[idx].tat;
        sum_wait += p[idx].wait_time;
    }

    printf("Average Turn Around Time: %.2f\n",(double)sum_tat/max);
    printf("Average Wait Time: %.2f\n",(double)sum_wait/max);
}
int main(){
    process p[max];
    for(int i=0;i<max;i++){
        scanf("%s",p[i].name);
        scanf("%d",&p[i].arrival_time);
        scanf("%d",&p[i].burst_time);
        scanf("%d",&p[i].priority);
        getchar();
        p[i].completion_time=-1;
        p[i].tat=-1;
        p[i].wait_time=-1;
        p[i].done=0;
    }
    order(p);
    calculate(p);
    printf("\nFinal Process Table:\n");
    printf("%-8s %-8s %-8s %-8s %-12s %-8s %-8s\n", "Name", "Arrive", "Burst","Priority" "Complete", "TAT", "Wait");
    for(int i=0;i<max;i++){
        printf("%-8s %-8d %-8d %-8d %-12d %-8d %-8d\n", 
               p[i].name, p[i].arrival_time, p[i].burst_time,p[i].priority,
               p[i].completion_time, p[i].tat, p[i].wait_time);
    }
}