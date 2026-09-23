#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

class CpuMonitor {
private:
    unsigned long long prevIdle;
    unsigned long long prevTotal;

public:
    CpuMonitor();

    double getUsage();
};

#endif
