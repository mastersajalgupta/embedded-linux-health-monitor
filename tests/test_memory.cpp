#include <iostream>
#include "memory_monitor.h"

int main() {

    MemoryMonitor memory;

    double usage = memory.getUsage();

    std::cout << "Memory Usage: " << usage << "%" << std::endl;

    if (usage >= 0 && usage <= 100) {
        std::cout << "Memory test PASSED" << std::endl;
        return 0;
    }

    std::cout << "Memory test FAILED" << std::endl;
    return 1;
}
