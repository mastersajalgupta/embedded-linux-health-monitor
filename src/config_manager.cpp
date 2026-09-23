#include "config_manager.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

bool ConfigManager::load(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    try {
        json config;
        file >> config;

        cpuThreshold = config.at("cpu_threshold");
        memoryThreshold = config.at("memory_threshold");
        diskThreshold = config.at("disk_threshold");
        temperatureThreshold = config.at("temperature_threshold");
        checkInterval = config.at("check_interval");

        return true;
    }
    catch (...) {
        return false;
    }
}

int ConfigManager::getCpuThreshold() const {
    return cpuThreshold;
}

int ConfigManager::getMemoryThreshold() const {
    return memoryThreshold;
}

int ConfigManager::getDiskThreshold() const {
    return diskThreshold;
}

int ConfigManager::getTemperatureThreshold() const {
    return temperatureThreshold;
}

int ConfigManager::getCheckInterval() const {
    return checkInterval;
}