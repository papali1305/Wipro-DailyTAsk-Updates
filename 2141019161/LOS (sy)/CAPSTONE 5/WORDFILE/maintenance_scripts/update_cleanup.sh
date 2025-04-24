#!/bin/bash

echo "Updating system..."
sudo apt update && sudo apt upgrade -y

echo "Cleaning up unused packages..."
sudo apt autoremove -y
sudo apt autoclean

echo "System update and cleanup completed."
