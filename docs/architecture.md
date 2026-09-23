# System Architecture

## 1. Overview

The Embedded Linux Device Health Monitor & Auto-Recovery Agent is a C++17 application designed to continuously monitor the health of a Linux-based device.

The application collects system health information at regular intervals and checks the monitored values against configurable thresholds.

It also monitors a critical Linux service and automatically attempts to restart it when the service becomes unavailable.

## 2. High-Level Architecture

```text
                         Linux Device
                              |
                              v
                 +-------------------------+
                 |   Health Monitor Agent  |
                 |        C++17            |
                 +------------+------------+
                              |
        +---------------------+----------------------+
        |          |          |          |           |
        v          v          v          v           v
      CPU        Memory      Disk    Temperature   Network
   /proc/stat  /proc/meminfo  statvfs  /sys/class/thermal
                                                       
                              |
                              v
                    +-------------------+
                    | Service Monitor   |
                    +---------+---------+
                              |
                              v
                       mysql.service
                              |
                         If stopped
                              |
                              v
                   systemctl restart
                              |
                              v
                         Recovery
```

## 3. Main Components

### ConfigManager

Responsible for loading monitoring thresholds and the monitoring interval from:

```text
config/health_monitor.json
```

It provides configuration values to the main monitoring loop.

### Logger

Provides simple logging for:

* Information messages
* Warning messages
* Error messages

When the application runs under systemd, these messages are available through `journalctl`.

### CpuMonitor

Reads CPU statistics from:

```text
/proc/stat
```

Two samples are compared to calculate CPU utilization over the sampling interval.

### MemoryMonitor

Reads:

```text
/proc/meminfo
```

The monitor uses `MemTotal` and `MemAvailable` to calculate the percentage of memory currently in use.

### DiskMonitor

Uses the Linux `statvfs()` system call to obtain filesystem statistics for `/`.

The monitor calculates the percentage of disk space currently used.

### TemperatureMonitor

Checks:

```text
/sys/class/thermal/
```

When a thermal zone is available, the temperature is read from its `temp` file.

If no usable thermal zone is available, the monitor reports that the sensor is unavailable.

### NetworkMonitor

Uses Linux network interface information through `getifaddrs()`.

The monitor ignores the loopback interface and checks for an active interface with an IPv4 or IPv6 address.

### ServiceMonitor

Uses `systemctl` to check and restart the monitored Linux service.

The current critical service is:

```text
mysql.service
```

The service monitor provides two main operations:

```text
isRunning()
restart()
```

## 4. Monitoring Flow

The main application performs the following sequence:

```text
Start
  |
  v
Load JSON configuration
  |
  v
Initialize monitors
  |
  v
Take initial CPU sample
  |
  v
Wait for configured interval
  |
  v
Collect health information
  |
  +---- CPU
  |
  +---- Memory
  |
  +---- Disk
  |
  +---- Temperature
  |
  +---- Network
  |
  +---- MySQL service
  |
  v
Compare values with thresholds
  |
  v
Log warnings/errors
  |
  v
If MySQL is stopped
  |
  v
Attempt automatic restart
  |
  v
Repeat continuously
```

## 5. Configuration Flow

```text
health_monitor.json
        |
        v
ConfigManager
        |
        v
Main Monitoring Loop
        |
        +--> CPU threshold
        +--> Memory threshold
        +--> Disk threshold
        +--> Temperature threshold
        +--> Check interval
```

## 6. Auto-Recovery Flow

The service recovery mechanism works as follows:

```text
Check mysql.service
        |
        v
Is service active?
     /       \
   YES        NO
    |          |
    v          v
Continue    Log error
monitoring      |
                v
        Attempt restart
                |
          +-----+-----+
          |           |
       Success       Failure
          |           |
          v           v
      Log success   Log error
```

## 7. systemd Integration

The monitor runs as a systemd service:

```text
device-health-monitor.service
```

The service configuration provides:

* Automatic startup during boot
* Root privileges required for service recovery
* Automatic restart if the monitor process exits
* Dependency ordering after networking and MySQL

The service is configured with:

```text
Restart=always
RestartSec=5
```

This provides process-level recovery for the monitoring agent itself.

## 8. Data Sources

| Monitor     | Linux Source / API    |
| ----------- | --------------------- |
| CPU         | `/proc/stat`          |
| Memory      | `/proc/meminfo`       |
| Disk        | `statvfs()`           |
| Temperature | `/sys/class/thermal/` |
| Network     | `getifaddrs()`        |
| Service     | `systemctl`           |

## 9. Design Approach

The project uses separate monitor classes for each health metric.

This provides:

* Clear separation of responsibilities
* Easier testing
* Easier maintenance
* Simple integration with the main monitoring loop
* Independent development of monitoring components

The application follows a modular structure with separate header and implementation files.

## 10. Security and Permissions

The monitor requires root privileges when running as a systemd service because restarting a system service normally requires elevated permissions.

The systemd service therefore runs as:

```text
User=root
```

The application does not modify network configuration or perform automatic network recovery. Network monitoring is limited to detecting and reporting connectivity status.

## 11. Current Limitation

The development Ubuntu system does not expose a usable thermal zone under:

```text
/sys/class/thermal/
```

Therefore the temperature monitor reports:

```text
Temperature: Sensor unavailable
```

The implementation is designed to use a thermal sensor automatically when a supported thermal zone becomes available.
