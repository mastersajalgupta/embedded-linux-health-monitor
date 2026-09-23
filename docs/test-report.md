# Test Report

## Embedded Linux Device Health Monitor & Auto-Recovery Agent

## 1. Test Environment

| Item             | Details            |
| ---------------- | ------------------ |
| Operating System | Ubuntu 24.04.3 LTS |
| Architecture     | x86_64             |
| Language         | C++17              |
| Build System     | CMake              |
| Compiler         | g++                |
| Init System      | systemd            |
| Critical Service | MySQL              |
| Configuration    | JSON               |

## 2. Unit Tests

### CPU Monitor Test

Test command:

```text
./build/test_cpu
```

Expected behavior:

* CPU usage should be between 0% and 100%.
* Test should report `CPU test PASSED`.

Result:

```text
CPU test PASSED
```

Status: **PASSED**

---

### Memory Monitor Test

Test command:

```text
./build/test_memory
```

Observed result:

```text
Memory Usage: 11.7385%
Memory test PASSED
```

Status: **PASSED**

---

### Disk Monitor Test

Test command:

```text
./build/test_disk
```

Expected behavior:

* Disk usage should be between 0% and 100%.
* Test should report `Disk test PASSED`.

Status: **PASSED**

---

### Configuration Manager Test

Test command:

```text
./build/test_config
```

Observed configuration:

```text
CPU Threshold: 80
Memory Threshold: 80
Disk Threshold: 85
Temperature Threshold: 75
Check Interval: 5
Config test PASSED
```

Status: **PASSED**

---

### Service Monitor Test

Test command:

```text
./build/test_service_monitor
```

Observed result:

```text
MySQL service is running
Service monitor test PASSED
```

Status: **PASSED**

## 3. Continuous Monitoring Test

The complete health monitor was executed as a systemd service.

Command:

```text
sudo systemctl status device-health-monitor
```

Observed status:

```text
Active: active (running)
```

The monitor continuously reported:

```text
CPU Usage: 0.249813%
Memory Usage: 7.16648%
Disk Usage: 5.40886%
Temperature: Sensor unavailable
Network: Connected
```

Status: **PASSED**

## 4. Temperature Monitoring Test

The application checks:

```text
/sys/class/thermal/
```

The development system did not expose a usable thermal zone.

Therefore the application correctly reported:

```text
Temperature: Sensor unavailable
```

No artificial temperature value was generated.

Status: **PASSED**

Note: This indicates correct handling of an unavailable sensor rather than a failure of the monitoring application.

## 5. Network Monitoring Test

The network monitor checks active non-loopback interfaces.

Observed result:

```text
Network: Connected
```

Status: **PASSED**

## 6. MySQL Auto-Recovery Test

The MySQL service was intentionally stopped to simulate a service failure.

Test command:

```text
sudo systemctl stop mysql.service
```

The health monitor detected the failure during its next monitoring cycle.

Observed log:

```text
[ERROR] MySQL service is not running
[INFO] Attempting to restart MySQL service
[INFO] MySQL service restarted successfully
```

The service was then verified using:

```text
systemctl is-active mysql.service
```

Expected result:

```text
active
```

Status: **PASSED**

## 7. Failure Simulation Script

The project includes:

```text
scripts/simulate_failure.sh
```

The script intentionally stops the MySQL service:

```text
sudo systemctl stop mysql.service
```

The health monitor then detects the failure and performs automatic recovery.

This provides a repeatable demonstration of the project's auto-recovery mechanism.

Status: **PASSED**

## 8. systemd Test

The health monitor was installed as:

```text
device-health-monitor.service
```

The service was enabled to start automatically:

```text
sudo systemctl enable device-health-monitor
```

The service was started using:

```text
sudo systemctl start device-health-monitor
```

Status verification:

```text
sudo systemctl status device-health-monitor
```

Observed:

```text
Active: active (running)
```

Status: **PASSED**

## 9. Logging Test

The monitor's logs were viewed using:

```text
sudo journalctl -u device-health-monitor -n 20 --no-pager
```

The journal contained:

* CPU readings
* Memory readings
* Disk readings
* Temperature status
* Network status
* Service failure messages
* Auto-recovery messages

Status: **PASSED**

## 10. Test Summary

| Test                     | Result |
| ------------------------ | ------ |
| CPU monitoring           | PASSED |
| Memory monitoring        | PASSED |
| Disk monitoring          | PASSED |
| Temperature handling     | PASSED |
| Network monitoring       | PASSED |
| Configuration loading    | PASSED |
| MySQL service monitoring | PASSED |
| MySQL auto-recovery      | PASSED |
| Failure simulation       | PASSED |
| systemd integration      | PASSED |
| Journal logging          | PASSED |

## 11. Overall Result

The core functionality of the Embedded Linux Device Health Monitor & Auto-Recovery Agent was successfully implemented and tested.

The system can continuously monitor CPU, memory, disk, temperature availability, network connectivity, and a critical Linux service.

The automatic recovery mechanism was successfully demonstrated by intentionally stopping MySQL and verifying that the monitoring agent detected the failure and restarted the service.

The project is therefore ready for the remaining installation-script and final documentation work.

