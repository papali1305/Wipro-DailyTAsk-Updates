#include<iostream>
using namespace std;
#include<sys/sysinfo.h>
void displaymemoryinfo(){
	struct sysinfo info;
	if(sysinfo(&info)==0){
		cout << "total ram:" << info.totalram/(1024*1024) << "MB\n";
		cout << "total ram:" << info.totalram/(1021*1024*1024) << "GB\n";
		cout << "total ram:" << info.freeram/(1024*1024) << "MB\n";
		cout << "total ram:" << info.freeram/(1021*1024*1024) << "GB\n";

	}
}
int main(){
	displaymemoryinfo();
	return 0;
}
