#include<stdio.h>
int main(){
printf("enter no of processes:\n");
int n;
scanf("%d",&n);
char p[n];
printf("enter process names\n");
for(int i=0;i<n;i++){
scanf(" %c",&p[i]);
}
printf("enter no of available resource for resource A\n");
int a;
scanf("%d",&a);
printf("enter no of available resource for resource B\n");
int b;
scanf("%d",&b);
printf("enter no of available resource for resource C\n");
int c;
scanf("%d",&c);
int alloc[n][3];
printf("enter allocation matrix\n");
for(int i=0;i<n;i++){
for(int j=0;j<3;j++){
scanf("%d",&alloc[i][j]);
}
}
int max[n][3];
printf("enter max  matrix\n");
for(int i=0;i<n;i++){
for(int j=0;j<3;j++){
scanf("%d",&max[i][j]);
}
}
int need[n][3];
for(int i=0;i<n;i++){
for(int j=0;j<3;j++){
need[i][j]=max[i][j]-alloc[i][j];
}
}
char seq[n];
int k=0;
int finish[n];
for(int i=0;i<n;i++){
finish[i]=0;
}
int count=0;
while(count<n){
int found=0;
for(int j=0;j<n;j++){
if(!finish[j]){
if(a>=need[j][0]&&b>=need[j][1]&&c>=need[j][2]){
seq[k++]=p[j];
a=a+alloc[j][0];
b=b+alloc[j][1];
c=c+alloc[j][2];
finish[j]=1;
found=1;
count++;
}
}
}
if(!found){break;}
}
if(count==n){
printf("safe sequence:\n");
for(int i=0;i<n;i++){
printf("%c ",seq[i]);
}
}
else{
printf("no safe sequence\n");
}
printf("\ncheck request:\n");
char pr;
printf("enter process name:\n");
scanf(" %c",&pr);
int d,e,f;
printf("enter request for A B C:\n");
scanf("%d%d%d",&d,&e,&f);
int idx=-1;
for(int i=0;i<n;i++){
if(p[i]==pr){idx=i;break;}
}
if(idx==-1){printf("invalid process\n");return 0;}
if(d<=need[idx][0]&&e<=need[idx][1]&&f<=need[idx][2]&&d<=a&&e<=b&&f<=c){
a-=d;b-=e;c-=f;
alloc[idx][0]+=d;alloc[idx][1]+=e;alloc[idx][2]+=f;
need[idx][0]-=d;need[idx][1]-=e;need[idx][2]-=f;
for(int i=0;i<n;i++){finish[i]=0;}k=0;count=0;
while(count<n){
int found=0;
for(int j=0;j<n;j++){
if(!finish[j]){
if(a>=need[j][0]&&b>=need[j][1]&&c>=need[j][2]){
seq[k++]=p[j];
a+=alloc[j][0];
b+=alloc[j][1];
c+=alloc[j][2];
finish[j]=1;
found=1;
count++;
}
}
}
if(!found){break;}
}
if(count==n){
printf("request can be granted\nnew safe sequence:\n");
for(int i=0;i<n;i++){
printf("%c ",seq[i]);
}
}
else{
printf("request cannot be granted safely\n");
}
}
else{
printf("request not valid\n");
}
return 0;
}
