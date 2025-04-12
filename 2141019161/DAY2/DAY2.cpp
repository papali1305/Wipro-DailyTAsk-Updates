#include<iostream>
#include<fstream>
#include<sstream>
#include<thread>
#include<chrono>
using  namespace std;
struct CPUData{
long user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
};

CPUData getCPUData(){
ifstream file("/proc/stat");
string line;
CPUData cpu = {};

if(file.is_open()){
getline(file, line);
istringstream ss(line);  
string cpuLabel;

ss >> cpuLabel >> cpu.user >> cpu.nice >> cpu.system >> cpu.idle >> cpu.iowait >> cpu.irq >> cpu.softirq >> cpu.steal >> cpu.guest >> cpu.guest_nice;
}
return cpu;
}
double calculateCPUUsage(CPUData prev, CPUData current){
long previdle = prev.idle+prev.iowait;
long curridle = current.idle + current.iowait;
long prevTotal = prev.user + prev.nice + prev.system + prev.idle + prev.iowait + prev.irq + prev.softirq + prev.steal + prev.guest + prev.guest_nice;
long currTotal = current.user + current.nice + current.system + current.idle + current.iowait + current.irq + current.softirq + current.steal + current.guest + current.guest_nice;
long totaldiff = currTotal - prevTotal;
long ideldiff = curridle - previdle;

return (totaldiff  - ideldiff) * 100.0/totaldiff;
}
int main()
{
CPUData cpu = getCPUData();
cout << "User Time: " << cpu.user << "\n";
cout << "Nice Time: " << cpu.nice << "\n";
cout << "System Time: " << cpu.system << "\n";
cout << "Idle Time: " << cpu.idle << "\n";
cout << "Iowait Time: " << cpu.iowait << "\n";
cout << "Irq Time: " << cpu.irq << "\n";
cout << "Softirq Time: " << cpu.softirq << "\n";
cout << "Steal Time: " << cpu.steal << "\n";
cout << "Guest Time: " << cpu.guest << "\n";
cout << "Guest_nice Time: " << cpu.guest_nice << "\n";

long totalcputime = cpu.user + cpu.nice + cpu.system + cpu.idle + cpu.iowait + cpu.irq + cpu.softirq + cpu.steal + cpu.guest + cpu.guest_nice;
cout << "Total CPU Time : " << totalcputime << endl;
long idel_time =cpu.idle + cpu.iowait;
long Active_time= totalcputime - cpu.idle;
CPUData prevData = getCPUData();
this_thread::sleep_for(chrono::seconds(1));
CPUData currData = getCPUData();
double cpu_usage = calculateCPUUsage(prevData, currData);
cout << cpu_usage << "%" << endl;
return 0;
}







