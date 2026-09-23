cmake_minimum_required(VERSION 3.16)

project(device_health_monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(device-health-monitor
    src/main.cpp
    src/config_manager.cpp
    src/logger.cpp
    src/cpu_monitor.cpp
   src/memory_monitor.cpp
   src/disk_monitor.cpp
)
