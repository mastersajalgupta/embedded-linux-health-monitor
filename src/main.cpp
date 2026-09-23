#include <iostream>
#include <thread>
#include <chrono>
#include "config_manager.h"
#include "logger.h"
#include "cpu_monitor.h"

int main() {

    Logger::info("Device Health Monitor Started");

    ConfigManager config;

    if (!config.load("../config/health_monitor.json")) {
        Logger::error("Failed to load configuration");
        return 1;
    }

    Logger::info("Configuration loaded successfully");

    CpuMonitor cpu;

    cpu.getUsage();

    for (int i = 0; i < 5; i++) {

        std::this_thread::sleep_for(std::chrono::seconds(1));

        double usage = cpu.getUsage();

        std::cout << "CPU Usage: " << usage << "%" << std::endl;

        if (usage >= config.getCpuThreshold()) {
            Logger::warning("CPU usage is high");
        }
    }

    Logger::info("CPU Monitor Test Completed");

    return 0;
}