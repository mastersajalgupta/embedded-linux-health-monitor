#include <iostream>
#include <unistd.h>
#include "cpu_monitor.h"

int main() {

    CpuMonitor cpu;

    cpu.getUsage();

    sleep(1);

    double usage = cpu.getUsage();

    std::cout << "CPU Usage: " << usage << "%" << std::endl;

    if (usage >= 0 && usage <= 100) {
        std::cout << "CPU test PASSED" << std::endl;
        return 0;
    }

    std::cout << "CPU test FAILED" << std::endl;
    return 1;
}
