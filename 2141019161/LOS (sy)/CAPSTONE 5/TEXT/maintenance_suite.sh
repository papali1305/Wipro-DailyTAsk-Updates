#!/bin/bash

while true; do
    echo ""
    echo "===== System Maintenance Menu ====="
    echo "1. Create Backup of Documents"
    echo "2. Run System Update and Cleanup"
    echo "3. Check System Logs for Issues"
    echo "4. Exit Program"
    echo "==================================="
    read -p "Enter your choice: " choice

    case $choice in
        1) bash backup.sh ;;
        2) bash update_cleanup.sh ;;
        3) bash log_monitor.sh ;;
        4) echo "Exiting..."; exit ;;
        *) echo "Invalid selection. Please try again." ;;
    esac
done
