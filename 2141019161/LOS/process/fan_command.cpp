#include<iostream>
#include<stdlib.h>
#include<unistd.h>
using namespace std;
int main(int argc, char const *argv[])
{
	pid_t childpid = 0;
	int i,n;
	n = atoi(argv[1]); 
	for(int i=1;i<n;i++){
		
		if(childpid == fork()){
			cout << "Child pid  " << getpid() << "from the parent pid " << getpid() << endl;
			
		}
	}

	return 0;
}
