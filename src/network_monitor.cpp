#include "network_monitor.h"

#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

bool NetworkMonitor::isConnected() {

    struct ifaddrs* interfaces = nullptr;

    if (getifaddrs(&interfaces) == -1) {
        return false;
    }

    bool connected = false;

    for (struct ifaddrs* current = interfaces;
         current != nullptr;
         current = current->ifa_next) {

        if (current->ifa_addr == nullptr) {
            continue;
        }

        if (!(current->ifa_flags & IFF_UP)) {
            continue;
        }

        std::string name = current->ifa_name;

        if (name == "lo") {
            continue;
        }

        int family = current->ifa_addr->sa_family;

        if (family == AF_INET || family == AF_INET6) {
            connected = true;
            break;
        }
    }

    freeifaddrs(interfaces);

    return connected;
}
