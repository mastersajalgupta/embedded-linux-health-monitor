#include <iostream>
#include <thread>
#include <chrono>

#include "config_manager.h"
#include "logger.h"
#include "cpu_monitor.h"
#include "memory_monitor.h"
#include "disk_monitor.h"
#include "temperature_monitor.h"
#include "network_monitor.h"
#include "service_monitor.h"

int main() {

    Logger::info("Device Health Monitor Started");

    ConfigManager config;

    if (!config.load("/home/sangam/projects/embedded-linux-health-monitor/config/health_monitor.json")) {
        Logger::error("Failed to load configuration");
        return 1;
    }

    CpuMonitor cpu;
    MemoryMonitor memory;
    DiskMonitor disk;
    TemperatureMonitor temperature;
    NetworkMonitor network;
    ServiceMonitor service;

    cpu.getUsage();

    while (true) {

        std::this_thread::sleep_for(
            std::chrono::seconds(config.getCheckInterval())
        );

        double cpuUsage = cpu.getUsage();
        double memoryUsage = memory.getUsage();
        double diskUsage = disk.getUsage();
        double temperatureValue = temperature.getTemperature();
        bool networkConnected = network.isConnected();

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

        if (networkConnected) {
            std::cout << "Network: Connected" << std::endl;
        } else {
            std::cout << "Network: Disconnected" << std::endl;
            Logger::error("Network connection is unavailable");
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

        if (!service.isRunning("mysql.service")) {

            Logger::error("MySQL service is not running");
            Logger::info("Attempting to restart MySQL service");

            if (service.restart("mysql.service")) {
                Logger::info("MySQL service restarted successfully");
            } else {
                Logger::error("Failed to restart MySQL service");
            }
        }

        std::cout << "--------------------------------" << std::endl;
    }

    return 0;
}
