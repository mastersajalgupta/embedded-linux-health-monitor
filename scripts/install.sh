#!/bin/bash

set -e

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
SERVICE_FILE="$PROJECT_DIR/systemd/device-health-monitor.service"
SYSTEMD_FILE="/etc/systemd/system/device-health-monitor.service"

echo "Building Device Health Monitor..."

mkdir -p "$PROJECT_DIR/build"

cd "$PROJECT_DIR/build"

cmake ..
cmake --build .

echo "Installing systemd service..."

sed "s|@PROJECT_DIR@|$PROJECT_DIR|g" "$SERVICE_FILE" | \
sudo tee "$SYSTEMD_FILE" > /dev/null

sudo systemctl daemon-reload
sudo systemctl enable device-health-monitor
sudo systemctl restart device-health-monitor

echo
echo "Device Health Monitor installed successfully."
echo
sudo systemctl status device-health-monitor --no-pager
