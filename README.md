# Embedded Linux Device Health Monitor & Auto-Recovery Agent

A Linux-based device health monitoring and auto-recovery system written in C++. The application continuously monitors important system resources and automatically recovers a critical Linux service when it becomes unavailable.

## Features

* CPU usage monitoring
* RAM usage monitoring
* Disk usage monitoring
* CPU/system temperature monitoring
* Network connectivity monitoring
* Critical Linux service monitoring
* Automatic MySQL service recovery
* Continuous background monitoring
* Configurable health thresholds using JSON
* Logging through systemd journal
* Automatic startup using systemd
* Automatic restart if the monitoring application crashes
* Failure simulation for testing
* Unit tests for core monitoring components

## Technologies

* C++17
* Linux / Ubuntu
* CMake
* systemd
* `/proc` filesystem
* `/sys` filesystem
* nlohmann/json
* GoogleTest
* Git / GitHub

## System Architecture

```text
                +----------------------+
                |   Health Monitor     |
                |       C++ App        |
                +----------+-----------+
                           |
          +----------------+----------------+
          |                |                |
          v                v                v
      CPU Monitor     Memory Monitor    Disk Monitor
          |                |                |
          +----------------+----------------+
                           |
          +----------------+----------------+
          |                |                |
          v                v                v
 Temperature Monitor  Network Monitor  Service Monitor
                                             |
                                             v
                                      MySQL Service
                                             |
                                      Auto-Recovery
```

## Monitored Components

### 1. CPU

CPU utilization is calculated using information from:

```text
/proc/stat
```

The monitor compares CPU statistics between two sampling intervals to calculate CPU usage.

### 2. Memory

Memory information is read from:

```text
/proc/meminfo
```

The application uses `MemTotal` and `MemAvailable` to calculate memory utilization.

### 3. Disk

Disk utilization is obtained using the Linux `statvfs()` system call for the root filesystem.

### 4. Temperature

The application checks:

```text
/sys/class/thermal/
```

If a thermal zone is available, the temperature is read and converted to degrees Celsius.

On systems where no temperature sensor is exposed, the monitor reports:

```text
Temperature: Sensor unavailable
```

No artificial temperature value is generated.

### 5. Network

Network interfaces are inspected using Linux network interface information.

The monitor checks whether an active non-loopback interface has an IPv4 or IPv6 address.

### 6. Critical Service

The project monitors:

```text
mysql.service
```

If MySQL stops running, the monitor automatically attempts to restart it.

## Auto-Recovery

The monitor periodically checks the MySQL service.

If the service becomes unavailable:

```text
[ERROR] MySQL service is not running
[INFO] Attempting to restart MySQL service
[INFO] MySQL service restarted successfully
```

The recovery operation is performed using:

```text
systemctl restart mysql.service
```

The health monitor runs as a root-owned systemd service so it has the required permission to restart the monitored service.

## Configuration

Monitoring thresholds are stored in:

```text
config/health_monitor.json
```

Example:

```json
{
    "cpu_threshold": 80,
    "memory_threshold": 80,
    "disk_threshold": 85,
    "temperature_threshold": 75,
    "check_interval": 5
}
```

### Configuration Parameters

| Parameter               | Description                             |
| ----------------------- | --------------------------------------- |
| `cpu_threshold`         | Maximum allowed CPU usage percentage    |
| `memory_threshold`      | Maximum allowed memory usage percentage |
| `disk_threshold`        | Maximum allowed disk usage percentage   |
| `temperature_threshold` | Maximum allowed temperature in Celsius  |
| `check_interval`        | Monitoring interval in seconds          |

## Project Structure

```text
embedded-linux-health-monitor/
│
├── README.md
├── CMakeLists.txt
├── .gitignore
│
├── config/
│   └── health_monitor.json
│
├── include/
│   ├── cpu_monitor.h
│   ├── memory_monitor.h
│   ├── disk_monitor.h
│   ├── temperature_monitor.h
│   ├── network_monitor.h
│   ├── service_monitor.h
│   ├── logger.h
│   └── config_manager.h
│
├── src/
│   ├── main.cpp
│   ├── cpu_monitor.cpp
│   ├── memory_monitor.cpp
│   ├── disk_monitor.cpp
│   ├── temperature_monitor.cpp
│   ├── network_monitor.cpp
│   ├── service_monitor.cpp
│   ├── logger.cpp
│   └── config_manager.cpp
│
├── systemd/
│   └── device-health-monitor.service
│
├── scripts/
│   ├── simulate_failure.sh
│   ├── install.sh
│   └── uninstall.sh
│
├── tests/
│   ├── test_cpu.cpp
│   ├── test_memory.cpp
│   ├── test_config.cpp
│   ├── test_disk.cpp
│   └── test_service_monitor.cpp
│
└── docs/
    ├── architecture.md
    └── test-report.md
```

## Build

Clone the repository:

```bash
git clone https://github.com/mastersajalgupta/embedded-linux-health-monitor.git
```

Enter the project:

```bash
cd embedded-linux-health-monitor
```

Create the build directory:

```bash
mkdir -p build
cd build
```

Configure:

```bash
cmake ..
```

Build:

```bash
cmake --build .
```

## Run Manually

From the project root:

```bash
./build/device-health-monitor
```

The monitor will continuously check system health.

Stop it with:

```text
Ctrl + C
```

## systemd Service

The project includes:

```text
systemd/device-health-monitor.service
```

Install the service:

```bash
sudo cp systemd/device-health-monitor.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable device-health-monitor
sudo systemctl start device-health-monitor
```

Check status:

```bash
sudo systemctl status device-health-monitor
```

View logs:

```bash
sudo journalctl -u device-health-monitor -f
```

View recent logs:

```bash
sudo journalctl -u device-health-monitor -n 20 --no-pager
```

Stop the service:

```bash
sudo systemctl stop device-health-monitor
```

## Failure Simulation

The project includes a script for demonstrating automatic recovery.

Run:

```bash
./scripts/simulate_failure.sh
```

The script stops MySQL:

```text
mysql.service
```

The health monitor detects the failure and automatically restarts the service.

Verify the service:

```bash
systemctl is-active mysql.service
```

Expected:

```text
active
```

Check the monitor logs:

```bash
sudo journalctl -u device-health-monitor -n 20 --no-pager
```

Expected recovery messages:

```text
[ERROR] MySQL service is not running
[INFO] Attempting to restart MySQL service
[INFO] MySQL service restarted successfully
```

## Unit Tests

The project contains tests for:

* CPU monitoring
* Memory monitoring
* Disk monitoring
* Configuration loading
* MySQL service monitoring

Example:

```bash
./build/test_cpu
./build/test_memory
./build/test_disk
./build/test_config
./build/test_service_monitor
```

A successful test prints:

```text
test PASSED
```

## Example Monitoring Output

```text
CPU Usage: 0.249813%
Memory Usage: 7.16648%
Disk Usage: 5.40886%
Temperature: Sensor unavailable
Network: Connected
--------------------------------
```

When the monitored service fails:

```text
[ERROR] MySQL service is not running
[INFO] Attempting to restart MySQL service
[INFO] MySQL service restarted successfully
```

## Current System Limitation

The Ubuntu test system does not currently expose a usable thermal zone under:

```text
/sys/class/thermal/
```

Therefore the monitor reports:

```text
Temperature: Sensor unavailable
```

The temperature-monitoring code is designed to automatically use a thermal zone when one is available.

## Learning Objectives

This project demonstrates practical Linux system programming concepts including:

* Linux `/proc` filesystem
* Linux `/sys` filesystem
* System resource monitoring
* Network interface inspection
* Linux services
* `systemctl`
* systemd service management
* Automatic service recovery
* C++17
* CMake
* JSON configuration
* Linux permissions
* Logging with journalctl
* Unit testing
* Git and GitHub

## Project Status

Core monitoring and auto-recovery functionality has been implemented and tested.

Implemented:

* CPU monitoring
* Memory monitoring
* Disk monitoring
* Temperature monitoring
* Network monitoring
* MySQL service monitoring
* Automatic service recovery
* Continuous background execution
* systemd integration
* JSON configuration
* Failure simulation
* Unit tests

The project is being prepared as a complete GitHub-ready Linux systems project.
