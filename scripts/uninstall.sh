#!/bin/bash

set -e

echo "Stopping Device Health Monitor..."

sudo systemctl stop device-health-monitor || true

echo "Disabling Device Health Monitor..."

sudo systemctl disable device-health-monitor || true

echo "Removing systemd service..."

sudo rm -f /etc/systemd/system/device-health-monitor.service

sudo systemctl daemon-reload

echo
echo "Device Health Monitor uninstalled successfully."
