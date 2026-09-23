#include "disk_monitor.h"
#include <sys/statvfs.h>

double DiskMonitor::getUsage() {

    struct statvfs filesystem;

    if (statvfs("/", &filesystem) != 0) {
        return -1;
    }

    unsigned long long total =
        filesystem.f_blocks * filesystem.f_frsize;

    unsigned long long available =
        filesystem.f_bavail * filesystem.f_frsize;

    unsigned long long used = total - available;

    if (total == 0) {
        return -1;
    }

    return (100.0 * used) / total;
}
