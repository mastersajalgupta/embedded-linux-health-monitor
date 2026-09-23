#include <iostream>
#include <thread>
#include <chrono>
#include "config_manager.h"
#include "logger.h"
#include "cpu_monitor.h"
#include "memory_monitor.h"

int main() {

    Logger::info("Device Health Monitor Started");

    ConfigManager config;

    if (!config.load("../config/health_monitor.json")) {
        Logger::error("Failed to load configuration");
        return 1;
    }

    Logger::info("Configuration loaded successfully");

    CpuMonitor cpu;
    MemoryMonitor memory;

    cpu.getUsage();

    for (int i = 0; i < 5; i++) {

        std::this_thread::sleep_for(std::chrono::seconds(1));

        double cpuUsage = cpu.getUsage();
        double memoryUsage = memory.getUsage();

        std::cout << "CPU Usage: "
                  << cpuUsage << "%" << std::endl;

        std::cout << "Memory Usage: "
                  << memoryUsage << "%" << std::endl;

        if (cpuUsage >= config.getCpuThreshold()) {
            Logger::warning("CPU usage is high");
        }

        if (memoryUsage >= config.getMemoryThreshold()) {
            Logger::warning("Memory usage is high");
        }

        std::cout << std::endl;
    }

    Logger::info("CPU and Memory Monitor Test Completed");

    return 0;
}
