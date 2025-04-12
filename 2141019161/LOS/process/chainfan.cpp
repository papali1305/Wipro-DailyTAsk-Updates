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
			if(i == n-1){
				for(int i=1;i<n;i++){
					if(fork() == 0){
						cout << "Child pid  " << getpid() << "from the parent pid " << getpid() << endl;
						exit(0);
					}
				}
			}
			
			cout << "Child process ID is " << getpid() << "\n";
		}
	}
	return 0;
}
