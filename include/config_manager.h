#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

class ConfigManager {
private:
    int cpuThreshold;
    int memoryThreshold;
    int diskThreshold;
    int temperatureThreshold;
    int checkInterval;

public:
    bool load(const std::string& filename);

    int getCpuThreshold() const;
    int getMemoryThreshold() const;
    int getDiskThreshold() const;
    int getTemperatureThreshold() const;
    int getCheckInterval() const;
};

#endif