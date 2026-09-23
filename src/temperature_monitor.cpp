#include "temperature_monitor.h"
#include <fstream>
#include <string>
#include <filesystem>

double TemperatureMonitor::getTemperature() {

    namespace fs = std::filesystem;

    try {

        for (const auto& entry : fs::directory_iterator("/sys/class/thermal")) {

            std::string path = entry.path().string();

            if (path.find("thermal_zone") == std::string::npos) {
                continue;
            }

            std::ifstream typeFile(path + "/type");
            std::ifstream tempFile(path + "/temp");

            if (!typeFile.is_open() || !tempFile.is_open()) {
                continue;
            }

            long long temperature;

            tempFile >> temperature;

            return temperature / 1000.0;
        }

    } catch (...) {
        return -1;
    }

    return -1;
}
