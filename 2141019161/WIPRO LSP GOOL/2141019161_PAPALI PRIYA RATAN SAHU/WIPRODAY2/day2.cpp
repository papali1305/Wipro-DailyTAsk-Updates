#include<iostream>     // for input output
#include<fstream>      // for file operations
#include<sstream>      // for parsing file content
#include<thread>       // to introduce delay
#include<chrono>       // to introduce delay
using namespace std;

struct CPUData {
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
};

CPUData getCPUData() {
    ifstream file("/proc/stat");
    string line;
    CPUData cpu = {}; // Initialize with zeros

    if (file.is_open()) {
        getline(file, line); // Read the first line
        istringstream ss(line);
        string cpuLabel;

        ss >> cpuLabel >> cpu.user >> cpu.nice >> cpu.system >> cpu.idle >> cpu.iowait
           >> cpu.irq >> cpu.softirq >> cpu.steal >> cpu.guest >> cpu.guest_nice;
    }

    return cpu;
}

double calculateCPUUsage(CPUData prev, CPUData current) {
    long prevIdle = prev.idle + prev.iowait;
    long currIdle = current.idle + current.iowait;

    long prevTotal = prev.user + prev.nice + prev.system + prev.idle + prev.iowait +
                     prev.irq + prev.softirq + prev.steal + prev.guest + prev.guest_nice;

    long currTotal = current.user + current.nice + current.system + current.idle + current.iowait +
                     current.irq + current.softirq + current.steal + current.guest + current.guest_nice;

    long totalDiff = currTotal - prevTotal;
    long idleDiff = currIdle - prevIdle;

    return (totalDiff - idleDiff) * 100.0 / totalDiff;
}

int main() {
    CPUData prevData = getCPUData();
    this_thread::sleep_for(chrono::seconds(1)); // Wait for 1 second
    CPUData currData = getCPUData();

    double cpuUsage = calculateCPUUsage(prevData, currData);

    cout << "CPU Usage: " << cpuUsage << "%\n";
    cout << "user: " << currData.user << "\n";
    cout << "nice: " << currData.nice << "\n";
    cout << "system: " << currData.system << "\n";
    cout << "idle: " << currData.idle << "\n";
    cout << "iowait: " << currData.iowait << "\n";
    cout << "irq: " << currData.irq << "\n";
    cout << "softirq: " << currData.softirq << "\n";
    cout << "steal: " << currData.steal << "\n";
    cout << "guest: " << currData.guest << "\n";
    cout << "guest_nice: " << currData.guest_nice << "\n";

    long total_cpu_time = currData.user + currData.nice + currData.system + currData.idle +
                          currData.iowait + currData.irq + currData.softirq + currData.steal +
                          currData.guest + currData.guest_nice;

    int idle_time = currData.idle + currData.iowait;
    cout << "Total CPU Time: " << total_cpu_time << "\n";
    cout << "Idle Time: " << idle_time << "\n";

    double cpu_usage = ((total_cpu_time - idle_time) / (double)total_cpu_time) * 100;
    cout << "Recalculated CPU Usage: " << cpu_usage << "%\n";

    return 0;
}
