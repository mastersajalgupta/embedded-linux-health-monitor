#include <iostream>
#include "config_manager.h"

int main() {

    ConfigManager config;

    if (!config.load("config/health_monitor.json")) {
        std::cout << "Config test FAILED" << std::endl;
        return 1;
    }

    std::cout << "CPU Threshold: "
              << config.getCpuThreshold() << std::endl;

    std::cout << "Memory Threshold: "
              << config.getMemoryThreshold() << std::endl;

    std::cout << "Disk Threshold: "
              << config.getDiskThreshold() << std::endl;

    std::cout << "Temperature Threshold: "
              << config.getTemperatureThreshold() << std::endl;

    std::cout << "Check Interval: "
              << config.getCheckInterval() << std::endl;

    std::cout << "Config test PASSED" << std::endl;

    return 0;
}
