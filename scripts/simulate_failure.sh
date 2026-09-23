#!/bin/bash

echo "Stopping MySQL service..."
sudo systemctl stop mysql.service

echo "MySQL service stopped."
echo "The Device Health Monitor should detect the failure and restart it."
