#!/bin/bash

LOG_PATH="/var/log/dpkg.log"
SEARCH_TERMS=("error" "fail" "critical")

# Clear or create the temporary output file
> /tmp/log_alerts.txt

echo "Scanning system logs for potential problems..."
for term in "${SEARCH_TERMS[@]}"; do
    grep -i "$term" "$LOG_PATH" >> /tmp/log_alerts.txt
done

if [[ -s /tmp/log_alerts.txt ]]; then
    echo "WARNING: Detected issues in the logs:"
    cat /tmp/log_alerts.txt
else
    echo "System logs are clear of critical problems."
fi
