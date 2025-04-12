//Dispaly CPU & memory usage per process
//sort processes by CPU/memory usages
// Total cputime = usertime+cputime(utime+ctime)
//Total CPUTime = utime + stime;
//Calculate process cpu usage
//CPU usage = (utime+stime)/systemupdate - starttime*100;
//g++ -std=c++17 activeprocess.cpp -o activeprocess

//Memory info is in /proc/[PID]/status
//cat /proc/1/status|grep VmRSS
// VmRSS(Resident Set Size) = Actual Ram used by the process.
//student@D001-37:~/Desktop/2141011082/LOS/Day03$ cat /proc/uptime
//2456.96(total system time) 29285.05(ideal time or total system unused)



#include<iostream>
#include<fstream>//read file(/proc data)
#include<sstream>//parse data
#include<vector>//store process
#include<algorithm>//sort the process
#include<filesystem>//

namespace fs = std::filesystem;

struct Processinfo
{
	int pid;
	std::string name;
	double cpuUsage;
	long memoryUsage;
};
std::string readFileValue(const std::string &path){
	std::ifstream file(path);
	std::string value;
	if(file.is_open()){
		std::getline(file,value);
	}
	return value;
}
double getSystemUptime(){
	std::ifstream file("/proc/uptime");
	double uptime;
	if(file.is_open()){
		file >> uptime;// extract uptime only,not ideal time
	}
	return uptime;
}
//Read 
Processinfo getProcessInfo(int pid,double systemUptime){
	Processinfo proc;
    proc.pid = pid;

	std::ifstream file("/proc/" + std::to_string(pid) + "/stat");
    std::string line;
    long utime = 0, stime = 0, starttime = 0;
    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream ss(line);
        std::string token;
        int count = 0;
        long utime,stime,starttime;
        while (ss >> token) {
            count++;
            if (count == 2) Processinfo.name = token;
            else if(count == 14) utime = std::stol(token);
            else if(count == 15) stime = std::stol(token);
            else if(count == 22) starttime = std::stol(token);
        }
    }

    std::ifstream memFile("/proc/" + std::to_string(pid) + "/status");
    if(memFile.is_open()){
    	std::string key,value,unit;
    	while(memFile >> key >> value >> unit){
    		if(key == "VmRSS:"){
    			Processinfo.memoryUsage = std::stol(value);
    			break;
    		}
    	}
    }
    long total_time = utime+stime;
    double seconds = systemUptime - (starttime/sysconf(_SC_CLK_TCK));
    Processinfo.cpuUsage = (total_time/sysconf(_SC_CLK_TCK))/seconds*100;

    //Uptime = 100000;
    //uttime+time =20;
    //starttime = 90000;
    //Elapsed time = 100000 - (90000/100) = 1000
    //CPU Usage = (20/100)/1000*100 = 0.2%;

    std::vector<Processinfo> getAllprocess();
	std::vector<Processinfo> processes;
	double systemUptime = getSystemUptime();
	for(const auto &entry :fs::directory_iterator("/proc")){
		if(entry.is_directory()){
			std::string filename = entry.path().filename().string();
			if(all_of(filename.begin(),filename.end(),::isdigit)){
				int pid = std::stol(filename);
				processes.push_back(getProcessInfo(pid,systemUptime));
			}
		}
	}
	return processes;
}


void sortProcesses(std::vector<Processinfo> &processes,bool sortByCPU){
	if(sortByCPU){
		sort(processes.begin(),processes.end(),[](const Processinfo &a,const Processinfo &b){
			return a.cpuUsage > b.cpuUsage;
		});
	}
	else{
		sort(processes.begin(),processes.end(),[](const Processinfo &a,const Processinfo &b){
			return a.memoryUsage > b.memoryUsage;
		});
	}
}

int  main()
{
	std::vector<Processinfo> processes = getAllprocess();
	sortProcesses(processes,true);
	std::cout << "PID\tCPU%\tMemory (kB)\tName\n"
	for(size_t i=0;i<std::min(processes.size(),size_t(10));++i){
		std::cout << processes[i].pid <<"\t"
		          << processes[i].cpuUsage <<"\t"
		          << processes[i].memoryUsage << "\t"
		          << processes[i].name << "\n";
	}
	return 0;
}