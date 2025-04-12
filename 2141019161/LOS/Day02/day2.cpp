#include<iostream>
#include<fstream>
#include<sstream>
#include<thread>
#include<chrono>
using namespace std;

struct CPUData
{
	long user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
};
CPUData getCPUData(){
	ifstream file("/proc/stat");//opening 
	string line;
	CPUData cpu = {};//Initialize struct with zero value

	if(file.is_open()){//Read first line
		getline(file,line);
		istringstream ss(line);
		string cpuLabel;

		ss >> cpuLabel >> cpu.user >> cpu.nice >> cpu.system >> cpu.idle >> cpu.iowait >> cpu.irq >> cpu.softirq >> cpu.steal >> cpu.guest >> cpu.guest_nice ;
 	}
 	return cpu;
}
double calculateCPUUsage(CPUData prev ,CPUData current){
	long prevldle = prev.idle+prev.iowait;
	long currldle = current.idle + current.iowait;
	long prevTotal = prev.user + prev.nice + prev.system + prev.idle + prev.iowait + prev.irq + prev.softirq + prev.steal + prev.guest + prev.guest_nice;
	long currTotal = current.user + current.nice + current.system + currldle + current.idle + current.softirq + current.steal;
	long totaldiff = currTotal - prevTotal;
	long ideldiff = currldle - prevldle;

	return (totaldiff - ideldiff) * 100.0/totaldiff;

}
int main(){
	CPUData cpu = getCPUData();
	cout << "User Time:" << cpu.user << endl;
	cout << "Nice Time:" << cpu.nice << endl;
	cout << "System Time:" << cpu.system << endl;
	cout << "Idel Time:" << cpu.idle << endl;
	cout << "Iowait Time:" << cpu.iowait << endl;
	cout << "Irq Time:" << cpu.irq << endl;
	cout << "Softirq Time:" << cpu.softirq << endl;
	cout << "Steal Time:" << cpu.steal << endl;
	cout << "Guest Time:" << cpu.guest << endl;
	cout << "Guest Nice Time:" << cpu.guest_nice << endl;

	long totalcputime = cpu.user + cpu.nice + cpu.system + cpu.idle + cpu.iowait + cpu.irq + cpu.softirq + cpu.steal;
    cout << "Toatal CPU Time : " << totalcputime << endl;

    long idel_time = cpu.idle + cpu.iowait;
    long Active_time = totalcputime - cpu.idle;

    CPUData prevData = getCPUData();
    this_thread::sleep_for(chrono::seconds(1));//wait for 1 second
    CPUData currData = getCPUData();

    double cpu_usage = calculateCPUUsage(prevData,currData);

    cout << cpu_usage << "%"<< endl;

	return 0;
}