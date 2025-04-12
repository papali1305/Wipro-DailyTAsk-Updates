#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<iostream>

using namespace std;
int main(){
	int pid;
	int n;
	cout <<"Enter the number of process :\n";
	cin >> n;
	for(int i=1;i<n;i++){
		pid = fork();
		if(pid > 0){
			cout << "Parent ID is " << getpid() << "\n";
			break;
		}
		else{
			cout << "Child process ID is " << getpid() << "\n";
		}
	}
	return 0;
}
