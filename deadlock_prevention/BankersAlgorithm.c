/*
-----------------------------------------------
   BANKER'S ALGORITHM – SAFETY SEQUENCE CHECK
-----------------------------------------------

This program implements the Safety Algorithm
from the Banker's Algorithm in Operating Systems.

It takes as input:
 - The number of processes and resources
 - Allocation matrix
 - Maximum matrix
 - Total instances of each resource

Then it calculates:
 - The Need matrix
 - The Available resources
 - The Safe Sequence (if system is in safe state)

If all processes can finish without leading to
deadlock, it prints the Safe Sequence.
Otherwise, it reports that the system is in
an UNSAFE state.

-----------------------------------------------*/



#include<stdio.h>
#define max 5  //no of processes
#define max_res 3 //no of resources

typedef struct process{
    char name[50];
    int allocation[max_res];
    int maximum[max_res];
    int need[max_res];
}process;

int main(){
    process p[max];
    for(int i=0;i<max;i++){
        printf("enter process name: \n");
        scanf("%s",p[i].name);
        for(int j=0;j<max_res;j++){
            printf("enter allocation for process %s for resource %d\n",p[i].name,j);
            scanf("%d",&p[i].allocation[j]);
        }
        for(int j=0;j<max_res;j++){
            printf("enter maximum for process %s for resource %d\n",p[i].name,j);
            scanf("%d",&p[i].maximum[j]);

            p[i].need[j]=p[i].maximum[j]-p[i].allocation[j];
        }
    }

    int total[max_res];
    for(int i=0;i<max_res;i++){
        printf("enter total amt of instances for resource %d\n",i);
        scanf("%d",&total[i]);
    }

    int available[max_res];
    for(int i=0;i<max_res;i++){
        int sum=0;
        for(int j=0;j<max;j++){
            sum+=p[j].allocation[i];
        }
        available[i]=total[i]-sum;
    }

    int checked[max]={0};

    process safe_sequence[max];
    int safe_index=0;

    int finish_count=0;
    int progress_made=1; //to detect if new process was finished in algorithm

    while(finish_count<max && progress_made){
        progress_made=0;
        for(int i=0;i<max;i++){
            if(checked[i]==0){   //checks if process is unfinished
                int can_run=1;


                for(int j=0;j<max_res;j++){
                    if(p[i].need[j]>available[j]){
                        can_run=0;
                        break;
                    }
                }

                if(can_run){
                    for(int j=0;j<max_res;j++){
                        available[j]+=p[i].allocation[j];
                    }

                    checked[i]=1;
                    safe_sequence[safe_index++]=p[i];
                    finish_count++;
                    progress_made=1;
                }
            }
        }
        if(!progress_made)break;
    }


    // Final result
    if (finish_count == max) {
        printf("\nSystem is in a SAFE state.\nSafe sequence: ");
        for (int i = 0; i < max; i++)
            printf("%s ", safe_sequence[i].name);
        printf("\n");
    } else {
        printf("\nSystem is in an UNSAFE state.\n");
    }

    return 0;
    
    
}