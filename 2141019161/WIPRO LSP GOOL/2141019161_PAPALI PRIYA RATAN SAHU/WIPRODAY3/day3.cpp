#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <filesystem>
#include <vector>
#include <dirent.h>
#include <string>
#include <cctype>
#include <cmath>

namespace fs = std::filesystem;

struct ProcessInfo {
    int pid;
    std::string name;
    double cpuUsage;
    long memoryUsage;
};

// Read single value from file
std::string readFileValue(const std::string &path) {
    std::ifstream file(path);
    std::string value;
    if (file.is_open()) {
        std::getline(file, value);
    }
    return value;
}

// Read system uptime
double getSystemUptime() {
    std::ifstream file("/proc/uptime");
    double uptime = 0;
    if (file.is_open()) {
        file >> uptime;
    }
    return uptime;
}

// Get info for a single process
ProcessInfo getProcessInfo(int pid, double systemUptime) {
    ProcessInfo pInfo;
    pInfo.pid = pid;

    std::ifstream file("/proc/" + std::to_string(pid) + "/stat");
    std::string line;
    long utime = 0, stime = 0, starttime = 0;

    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream ss(line);
        std::string token;
        int count = 0;

        while (ss >> token) {
            count++;
            if (count == 2)
                pInfo.name = token;
            else if (count == 14)
                utime = std::stol(token);
            else if (count == 15)
                stime = std::stol(token);
            else if (count == 22)
                starttime = std::stol(token);
        }

        // Get memory usage
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

        if (seconds > 0)
            pInfo.cpuUsage = (total_time / static_cast<double>(sysconf(_SC_CLK_TCK))) / seconds * 100.0;
        else
            pInfo.cpuUsage = 0.0;
    }

    return pInfo;
}

// Get all processes
std::vector<ProcessInfo> getAllProcesses() {
    std::vector<ProcessInfo> processes;
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

// Sort processes
void sortProcesses(std::vector<ProcessInfo> &processes, bool sortByCPU) {
    if (sortByCPU) {
        std::sort(processes.begin(), processes.end(), [](const ProcessInfo &a, const ProcessInfo &b) {
            return a.cpuUsage > b.cpuUsage;
        });
    } else {
        std::sort(processes.begin(), processes.end(), [](const ProcessInfo &a, const ProcessInfo &b) {
            return a.memoryUsage > b.memoryUsage;
        });
    }
}

// Main
int main() {
    std::vector<ProcessInfo> processes = getAllProcesses();
    sortProcesses(processes, true);  // true = sort by CPU

    std::cout << "PID\tCPU%\tMemory (kB)\tName\n";
    for (size_t i = 0; i < std::min(processes.size(), size_t(10)); ++i) {
        std::cout << processes[i].pid << "\t"
                  << processes[i].cpuUsage << "%\t"
                  << processes[i].memoryUsage << "\t"
                  << processes[i].name << "\n";
    }

    return 0;
}
