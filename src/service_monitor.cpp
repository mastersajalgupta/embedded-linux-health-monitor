#include "service_monitor.h"
#include <cstdlib>

bool ServiceMonitor::isRunning(const std::string& serviceName) {

    std::string command =
        "systemctl is-active --quiet " + serviceName;

    int result = std::system(command.c_str());

    return result == 0;
}

bool ServiceMonitor::restart(const std::string& serviceName) {

    std::string command =
        "systemctl restart " + serviceName;

    int result = std::system(command.c_str());

    return result == 0;
}
