#include <iostream>
#include "config_manager.h"
#include "logger.h"

int main() {

    Logger::info("Device Health Monitor Started");

    ConfigManager config;

    if (!config.load("../config/health_monitor.json")) {
        Logger::error("Failed to load configuration");
        return 1;
    }

    Logger::info("Configuration loaded successfully");

    std::cout << "CPU Threshold: "
              << config.getCpuThreshold() << "%" << std::endl;

    std::cout << "Memory Threshold: "
              << config.getMemoryThreshold() << "%" << std::endl;

    std::cout << "Disk Threshold: "
              << config.getDiskThreshold() << "%" << std::endl;

    std::cout << "Temperature Threshold: "
              << config.getTemperatureThreshold() << " C" << std::endl;

    std::cout << "Check Interval: "
              << config.getCheckInterval() << " seconds" << std::endl;

    Logger::info("Device Health Monitor Test Completed");

    return 0;
}