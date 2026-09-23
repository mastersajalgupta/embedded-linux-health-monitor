#include <iostream>
#include "service_monitor.h"

int main() {

    ServiceMonitor service;

    if (service.isRunning("mysql.service")) {
        std::cout << "MySQL service is running" << std::endl;
    } else {
        std::cout << "MySQL service is NOT running" << std::endl;
    }

    return 0;
}
