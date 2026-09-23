#include <iostream>
#include <thread>
#include <chrono>
#include "config_manager.h"
#include "logger.h"
#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "temperature_monitor.h"

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
    DiskMonitor disk;
    TemperatureMonitor temperature;

    cpu.getUsage();

    for (int i = 0; i < 5; i++) {

        std::this_thread::sleep_for(std::chrono::seconds(1));

        double cpuUsage = cpu.getUsage();
        double memoryUsage = memory.getUsage();
        double diskUsage = disk.getUsage();
        double temperatureValue = temperature.getTemperature();

        std::cout << "CPU Usage: "
                  << cpuUsage << "%" << std::endl;

        std::cout << "Memory Usage: "
                  << memoryUsage << "%" << std::endl;

        std::cout << "Disk Usage: "
                  << diskUsage << "%" << std::endl;

        if (temperatureValue >= 0) {
            std::cout << "Temperature: "
                      << temperatureValue << " C"
                      << std::endl;
        } else {
            std::cout << "Temperature: Sensor unavailable"
                      << std::endl;
        }

        if (cpuUsage >= config.getCpuThreshold()) {
            Logger::warning("CPU usage is high");
        }

        if (memoryUsage >= config.getMemoryThreshold()) {
            Logger::warning("Memory usage is high");
        }

        if (diskUsage >= config.getDiskThreshold()) {
            Logger::warning("Disk usage is high");
        }

        if (temperatureValue >= config.getTemperatureThreshold()) {
            Logger::warning("Temperature is high");
        }

        std::cout << std::endl;
    }

    Logger::info("CPU, Memory, Disk and Temperature Monitor Test Completed");

    return 0;
}
