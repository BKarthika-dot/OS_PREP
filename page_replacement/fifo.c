#include<stdio.h>
#define page_no 12
#define frame_no 3
int main(){

    int pages[page_no];
    int frames[frame_no];
    int no_of_hits=0;
    int t=0;

    for(int i=0;i<page_no;i++){
        scanf("%d",&pages[i]);
    }
    for(int i=0;i<frame_no;i++){
        frames[i]=-1;
    }
    for(int i=0;i<page_no;i++){
        int hit=0;
        for(int j=0;j<frame_no;j++){
            if(frames[j]==pages[i]){
                hit=1;
                break;
            }
        }
        if(!hit){
            frames[t]=pages[i];
            t=(t+1)%frame_no;
        }
        else{
            no_of_hits++;
        }
    }
    printf("No of hits: %d\n",no_of_hits);
    printf("No of page faults: %d\n",page_no-no_of_hits);
}