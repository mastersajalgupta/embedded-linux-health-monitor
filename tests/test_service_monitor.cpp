#include <iostream>
#include "service_monitor.h"

int main() {

    ServiceMonitor service;

    if (service.isRunning("mysql.service")) {
        std::cout << "MySQL service is running" << std::endl;
        std::cout << "Service monitor test PASSED" << std::endl;
        return 0;
    }

    std::cout << "MySQL service is NOT running" << std::endl;
    std::cout << "Service monitor test FAILED" << std::endl;

    return 1;
}
