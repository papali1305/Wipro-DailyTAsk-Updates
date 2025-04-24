#!/bin/bash

echo "Initiating system update..."
sudo apt update && sudo apt upgrade -y

echo "Performing system cleanup..."
sudo apt autoremove -y
sudo apt autoclean

echo "System update and cleanup finished!"
