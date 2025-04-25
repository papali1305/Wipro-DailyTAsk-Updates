#include<iostream>
using namespace std;
#include<sys/sysinfo.h>
void displayMemoryInfo(){
struct sysinfo info;//structure name info
if(sysinfo(&info)==0)//sysinfo structure 
{
cout<<"Total RAM:"<<info.totalram/(1024*1024)<<"MB\n";
cout<<"Total RAM:"<<info.totalram/(1024*1024*1024)<<"GB\n";
cout<<"Total RAM:"<<info.freeram/(1024*1024)<<"MB\n";
cout<<"Total RAM:"<<info.freeram/(1024*1024*1024)<<"GB\n";
}
}
int main(){
displayMemoryInfo();
return 0;
}

