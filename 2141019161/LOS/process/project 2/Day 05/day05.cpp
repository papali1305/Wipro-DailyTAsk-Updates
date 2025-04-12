//Write a parent child code where the child recives a string from user and share it to the
//parent using pipe.
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>


using namespace std;

int main()
{
	int fd[2];
	pid_t pid;
	char write_msg[] = "Hello from parent proccess ";
	char read_msg[100];
	pipe(fd);
	pid = fork();
	if(pid==0){
		//Child process
		close(fd[1]);
		read(fd[0],read_msg,sizeof(read_msg));
		printf("Child process recived message : %s\n" ,read_msg );
	}
	else{
		//Parent process
		close(fd[0]);
		write(fd[1],write_msg,sizeof(write_msg)+1);
		printf("Parent process send message : %s\n",write_msg);
		close(fd[1]);
		wait(NULL);
	}

	return 0;
}
