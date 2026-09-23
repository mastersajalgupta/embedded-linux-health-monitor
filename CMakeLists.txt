cmake_minimum_required(VERSION 3.16)

project(device_health_monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(device-health-monitor
    src/main.cpp
)