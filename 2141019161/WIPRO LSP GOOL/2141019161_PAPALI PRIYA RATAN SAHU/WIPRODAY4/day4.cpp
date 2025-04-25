 // Add the ability to the processes using kill(pid,SIGKILL)
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <dirent.h> // For directory handling
#include <cstring> // For strcmp
#include<csignal>// for kill() and SIGKILL

struct Processinfo {
    int pid;
    std::string name;
    double cpuUsage;
    long memoryUsage;
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
    double uptime;
    if (file.is_open()) {
        file >> uptime; // extract uptime only, not ideal time
    }
    return uptime;
}

Processinfo getProcessInfo(int pid, double systemUptime) {
    Processinfo pInfo;
    pInfo.pid = pid;

    std::ifstream statFile("/proc/" + std::to_string(pid) + "/stat");
    std::string line;
    long utime, stime, starttime;
    if (statFile.is_open()) {
        std::getline(statFile, line);
        std::istringstream ss(line);
        std::string token;
        int count = 0;
        while (ss >> token) {
            count++;
            if (count == 2) pInfo.name = token;
            else if (count == 14) utime = std::stol(token);
            else if (count == 15) stime = std::stol(token);
            else if (count == 22) starttime = std::stol(token);
        }
    }

    std::ifstream memFile("/proc/" + std::to_string(pid) + "/status");
    if (memFile.is_open()) {
        std::string key, value, unit;
        while (memFile >> key >> value >> unit) {
            if (key == "VmRSS:") {
                pInfo.memoryUsage = std::stol(value);
                break;
            }
        }
    }

    long total_time = utime + stime;
    double seconds = systemUptime - (starttime / sysconf(_SC_CLK_TCK));
    pInfo.cpuUsage = (static_cast<double>(total_time) / sysconf(_SC_CLK_TCK)) / seconds * 100;

    return pInfo;
}

std::vector<Processinfo> getAllProcesses() {
    std::vector<Processinfo> processes;
    double systemUptime = getSystemUptime();
    
    DIR *dir = opendir("/proc");
    struct dirent *entry;
    if (dir != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                // Check if the directory name is a number (PID)
                if (std::all_of(entry->d_name, entry->d_name + strlen(entry->d_name), ::isdigit)) {
                    int pid = std::stoi(entry->d_name);
                    processes.push_back(getProcessInfo(pid, systemUptime));
                }
            }
        }
        closedir(dir);
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

    std::vector<Processinfo> processes = getAllProcesses();
    sortProcesses(processes, true);
    std::cout << "PID\tCPU%\tMemory (kb)\tName\n";
    for (size_t i = 0; i < std::min(processes.size(), size_t(10)); i++) {
        std::cout << processes[i].pid << "\t"
                  << processes[i].cpuUsage << "%\t"
                  << processes[i].memoryUsage << "\t"
                  << processes[i].name << "\n";
    }
    int targetPid;

std::cout <<"Enter PID to kill :";
std::cin >> targetPid;

// use kill() function 

//signature : int kil(pid_t pid, int sig);

// kill return integer type data
//pid : Process ID
//ig : Signal to send (SIGKILL, SIGTERM, etc.)

if (targetPid>0)
{
if(kill(targetPid, SIGKILL)==0)
{
std::cout << "Process" << targetPid << "terminated successfully";
}
else
{
perror("Failed to kill process");
}
}
    
    // size_t

    return 0;
}
