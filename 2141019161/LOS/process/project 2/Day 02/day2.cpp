#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<vector>

int main(int argc, char* argv[]){
  if(argc<3){
    std::cerr  << "Usage: " << argv[0] << "<keyword> <file1> <file2> ...\n";
    return 1;
  }
  const char* keyword=argv[1];
  int num_files=argc-2;
  std::cout<< "Searching for keyword: " << keyword << "\n";
  std::cout << "Files to search : \n";
  for(int i=2; i<argc; ++i){
    std:: cout << "- " << argv[i] << "\n";
  }
  for(int i=0; i <num_files; ++i){
    const char* filename=argv[i+2];
    pid_t pid=fork();
    if(pid==0){
      execlp("grep", "grep", keyword, filename, filename, (char*)nullptr);
    }
    else if(pid>0){
    }
    else{
      perror("fork failed");
    }
    std::cout << "child process created for file: " << argv[i+2] << "\n";
    return 0;
  }
}
