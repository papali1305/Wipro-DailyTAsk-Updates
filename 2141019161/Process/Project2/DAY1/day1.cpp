// Setup Input and Process Spawning 
// Take  a list of file names and a keyword from the user . 
// create a child process (fork())  for each file.

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
using namespace std;
int main(int argc, char const *argv[])
{
	if(argc < 3){
		cerr << "usage : " << argv[0] << "<keyword> <file> <file2> ...\n";
		return 1;
	}
	const char* keyword = argv[1];
	int num_file = argc -2;
	cout << "Searching for keyword : " << keyword << "\n";
	cout << "Files to search : \n";
	for(int i=2;i < argc; ++i){
		cout << "_ " << argv[i] << "\n";
	}
	for(int i=2;i < num_file; ++i){
		pid_t pid = fork();
		if(pid == 0){
			cout << "Child process created for file : " << argv[i+2] << "\n";
			return 0;
		}
	}
	return 0;
}
