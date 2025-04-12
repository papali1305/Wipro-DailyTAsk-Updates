// cat proc/
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>


namespace fs = std::filesystem;

bool isNumber(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

std::string getProcessName(int pid) {
    std::ifstream file("/proc/" + std::to_string(pid) + "/stat");
    std::string line, processName;
    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream ss(line);
        std::string token;
        int count = 0;
        while (ss >> token) {
            count++;
            if (count == 2) {
                processName = token;
                break;
            }
        }
    }
    return processName;
}

int main() {
    std::cout << "Active processes :" << std::endl;
    for (const auto &entry : fs::directory_iterator("/proc")) {
        if (entry.is_directory()) {
            std::string filename = entry.path().filename().string();
            if (isNumber(filename)) {
                int pid = std::stoi(filename);
                std::string processName = getProcessName(pid);
                std::cout << "PID: " << pid << " | Name: " << processName << std::endl;
            }
        }
    }
    return 0;
}
