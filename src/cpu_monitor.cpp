#include "cpu_monitor.h"
#include <fstream>
#include <sstream>

CpuMonitor::CpuMonitor() {
    prevIdle = 0;
    prevTotal = 0;
}

double CpuMonitor::getUsage() {

    std::ifstream file("/proc/stat");

    if (!file.is_open()) {
        return -1;
    }

    std::string line;
    std::getline(file, line);

    std::stringstream ss(line);

    std::string cpu;
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;

    ss >> cpu >> user >> nice >> system >> idle
       >> iowait >> irq >> softirq >> steal;

    unsigned long long idleTime = idle + iowait;

    unsigned long long totalTime =
        user + nice + system + idle + iowait +
        irq + softirq + steal;

    if (prevTotal == 0) {
        prevIdle = idleTime;
        prevTotal = totalTime;
        return 0;
    }

    unsigned long long totalDifference = totalTime - prevTotal;
    unsigned long long idleDifference = idleTime - prevIdle;

    prevIdle = idleTime;
    prevTotal = totalTime;

    if (totalDifference == 0) {
        return 0;
    }

    double usage =
        100.0 * (totalDifference - idleDifference) /
        totalDifference;

    return usage;
}
