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
#include <iostream>
#include<fstream>//read file(/proc data)
#include<sstream>//parse data
#include<vector>//store process
#include<algorithm>//sort the process
#include <filesystem>
#include <unistd.h> // for sysconf
//sysconf(_SC_CLK_TCK)
#include<csignal>//for kill() and SIGKILL

namespace fs = std::filesystem;
using namespace std;

struct Processinfo {
    int pid;
    std::string name;
    double cpuUsage = 0;
    long memoryUsage = 0;
};

std::string readFileValue(const std::string &path) {
    std::ifstream file(path);
    std::string value;
    if (file.is_open()) {
        std::getline(file, value);
    }
    return value;
}

double getSystemUptime() {
    std::ifstream file("/proc/uptime");
    double uptime = 0;
    if (file.is_open()) {
        file >> uptime;
    }
    return uptime;
}

Processinfo getProcessInfo(int pid, double systemUptime) {
    Processinfo proc;
    proc.pid = pid;

    std::ifstream statFile("/proc/" + std::to_string(pid) + "/stat");
    std::string line;

    long utime = 0, stime = 0, starttime = 0;

    if (statFile.is_open()) {
        std::getline(statFile, line);
        std::istringstream ss(line);
        std::string token;
        for (int i = 1; ss >> token; ++i) {
            if (i == 2) proc.name = token;
            else if (i == 14) utime = std::stol(token);
            else if (i == 15) stime = std::stol(token);
            else if (i == 22) starttime = std::stol(token);
        }
    }

    std::ifstream memFile("/proc/" + std::to_string(pid) + "/status");
    if (memFile.is_open()) {
        std::string key, value, unit;
        while (memFile >> key >> value >> unit) {
            if (key == "VmRSS:") {
                proc.memoryUsage = std::stol(value);
                break;
            }
        }
    }

    long total_time = utime + stime;
    double seconds = systemUptime - (starttime / sysconf(_SC_CLK_TCK));
    if (seconds > 0) {
        proc.cpuUsage = ((total_time / (double)sysconf(_SC_CLK_TCK)) / seconds) * 100.0;
    }

    return proc;
}

std::vector<Processinfo> getAllprocess() {
    std::vector<Processinfo> processes;
    double systemUptime = getSystemUptime();

    for (const auto &entry : fs::directory_iterator("/proc")) {
        if (entry.is_directory()) {
            std::string filename = entry.path().filename().string();
            if (std::all_of(filename.begin(), filename.end(), ::isdigit)) {
                int pid = std::stoi(filename);
                processes.push_back(getProcessInfo(pid, systemUptime));
            }
        }
    }
    return processes;
}

void sortProcesses(std::vector<Processinfo> &processes, bool sortByCPU) {
    if (sortByCPU) {
        std::sort(processes.begin(), processes.end(), [](const Processinfo &a, const Processinfo &b) {
            return a.cpuUsage > b.cpuUsage;
        });
    } else {
        std::sort(processes.begin(), processes.end(), [](const Processinfo &a, const Processinfo &b) {
            return a.memoryUsage > b.memoryUsage;
        });
    }
}

int main() {
    vector<Processinfo> processes = getAllprocess();
    sortProcesses(processes, true); // Change to false to sort by memory usage

    cout << "PID\tCPU%\tMemory (kB)\tName\n";

    for (size_t i = 0; i < min(processes.size(), size_t(10)); ++i) {
            cout << processes[i].pid << "\t"
            << processes[i].cpuUsage << "\t"
            << processes[i].memoryUsage << "\t"
            << processes[i].name << "\n";
    }
    int targetPid;
    cout <<"Enter PID to Kill : " << endl;
    cin >> targetPid;
    //Signature : int kill(pid_t,int sig)
    if(targetPid){
        if(kill(targetPid,SIGKILL)==0){
            cout << "Process " << targetPid << " terminated successfully" << endl;
        }
        else{
            perror("Failed to kill Process");
        }
    }

    return 0;
}

