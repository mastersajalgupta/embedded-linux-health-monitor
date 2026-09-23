#include <iostream>
#include "disk_monitor.h"

int main() {

    DiskMonitor disk;

    double usage = disk.getUsage();

    std::cout << "Disk Usage: " << usage << "%" << std::endl;

    if (usage >= 0 && usage <= 100) {
        std::cout << "Disk test PASSED" << std::endl;
        return 0;
    }

    std::cout << "Disk test FAILED" << std::endl;
    return 1;
}
