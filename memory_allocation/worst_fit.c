#include<stdio.h>
#define max 3


int main(){
    printf("enter no of blocks ");
    int n;
    scanf("%d",&n);
    int blockSize[n];
    int blockAllocated[n];
    for(int i=0;i<n;i++){
        printf("enter size of block %d ",i);
        scanf("%d",&blockSize[i]);
        blockAllocated[i]=0;  //free block
    }


    int processSize[max],processAllocated[max];

    for(int i=0;i<max;i++){
        printf("enter size of process %d ",i);
        scanf("%d",&processSize[i]);
        processAllocated[i]=-1;  //not allocated process
    }

    for(int i=0;i<max;i++){
        int worstIdx=-1;
        for(int j=0;j<n;j++){
            if(!blockAllocated[j]&&blockSize[j]>=processSize[i]){
                if(worstIdx==-1 || blockSize[worstIdx]<blockSize[j]){
                    worstIdx=j;
                }
            }
        }
        if(worstIdx!=-1){
            processAllocated[i]=worstIdx;
            blockAllocated[worstIdx]=1;
        }
    }
    printf("Process Number\tProcess Size\tAllocated Block\n");

    for(int i=0;i<max;i++){
        printf("%d\t\t%d\t\t",i,processSize[i]);
        if(processAllocated[i]==-1){
            printf("Not allocated\n");
        }
        else{
            printf("%d\n",processAllocated[i]);
        }
    }
}