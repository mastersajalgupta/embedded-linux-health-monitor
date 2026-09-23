#include "memory_monitor.h"
#include <fstream>
#include <string>

double MemoryMonitor::getUsage() {

    std::ifstream file("/proc/meminfo");

    if (!file.is_open()) {
        return -1;
    }

    std::string name;
    unsigned long long value;
    std::string unit;

    unsigned long long total = 0;
    unsigned long long available = 0;

    while (file >> name >> value >> unit) {

        if (name == "MemTotal:") {
            total = value;
        }

        if (name == "MemAvailable:") {
            available = value;
        }

        if (total > 0 && available > 0) {
            break;
        }
    }

    if (total == 0) {
        return -1;
    }

    unsigned long long used = total - available;

    return (100.0 * used) / total;
}
