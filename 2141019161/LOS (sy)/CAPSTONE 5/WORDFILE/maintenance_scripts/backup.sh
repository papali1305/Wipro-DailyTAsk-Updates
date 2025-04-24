#!/bin/bash

SOURCE_DIR="$HOME/Documents"
DEST_DIR="$HOME/backup"

if [ ! -d "$SOURCE_DIR" ]; then
    echo "Source directory $SOURCE_DIR does not exist."
    echo "Creating the directory $SOURCE_DIR."
    mkdir -p "$SOURCE_DIR"
fi

mkdir -p "$DEST_DIR"
BACKUP_FILE="$DEST_DIR/backup_$(date +%Y%m%d_%H%M%S).tar.gz"
tar -czf "$BACKUP_FILE" "$SOURCE_DIR"

if [ $? -eq 0 ]; then
    echo "Backup completed successfully at: $BACKUP_FILE"
else
    echo "Backup failed. Please check the error above."
fi
