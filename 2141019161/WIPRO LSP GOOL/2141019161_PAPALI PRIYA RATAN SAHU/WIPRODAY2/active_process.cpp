#include<iostream>
#include<algorithm>
#include<filesystem>
#include<fstream>
#include<sstream>


namespace fs = std::filesystem;

bool isNumber(const std::string &s){//write the function to check if a directory is a number or not
   return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);// its will return true or false
}// check all the character in digits or not

std::string getProcessName(int pid){
          std::ifstream file("/proc/" + std::to_string(pid) + "/stat");//pid = process id
          
          std::string line, processName;
          
          if(file.is_open())
          {
              std::getline(file, line);//read the first line into the stream
              std::istringstream ss(line);//ss-it is an object
              std::string token;
              
              int count=0;
              while(ss >> token){ 
                 count++;
                 if(count==2){// it means it will show us the process name retrieve
                    processName=token;
                    break;
                    }
                   }
                  }
                  return processName;
                 } 
 int main()
 {
 std::cout <<"Active Processes:\n";
     for(const auto &entry : fs::directory_iterator("/proc")) //or("/proc")){ //
     
         if(entry.is_directory()) //files and folders both are present so we have to retrieve the folder
         {//check if directory is a process id // each process id has a directory
            std::string filename=entry.path().filename().string();
            
          if(isNumber(filename))
          {
            int pid=std::stoi(filename);// stoi- string to integer
            std::string processName=getProcessName(pid);
            std::cout << "PID:" << pid << " |Name: " << processName << "\n";
            }
            }
            return 0;
            }
 
