#!/bin/bash

LOGFILE="$HOME/maintenance.log"
SRC_DIR="$HOME/Documents"
BACKUP_DIR="$HOME/backup"

echo "[$(date)] Initiating backup process..." | tee -a "$LOGFILE"

# Ensure source directory exists
if [ ! -d "$SRC_DIR" ]; then
    echo "[$(date)] Source directory $SRC_DIR not found. Creating now..." | tee -a "$LOGFILE"
    mkdir -p "$SRC_DIR"
fi

# Make sure backup directory exists
mkdir -p "$BACKUP_DIR"

# Generate backup file name with timestamp
ARCHIVE_FILE="$BACKUP_DIR/backup_$(date +%Y%m%d_%H%M%S).tar.gz"

# Attempt to create the backup
if tar -czf "$ARCHIVE_FILE" "$SRC_DIR" 2>>"$LOGFILE"; then
    echo "[$(date)] Backup completed successfully: $ARCHIVE_FILE" | tee -a "$LOGFILE"
else
    echo "[$(date)] Error: Backup process failed." | tee -a "$LOGFILE"
fi

