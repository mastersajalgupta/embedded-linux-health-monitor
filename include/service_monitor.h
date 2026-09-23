#ifndef SERVICE_MONITOR_H
#define SERVICE_MONITOR_H

#include <string>

class ServiceMonitor {
public:
    bool isRunning(const std::string& serviceName);
};

#endif
