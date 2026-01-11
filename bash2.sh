#!/bin/bash

BACKUP="backup$1.sh"

cp "$0" "$BACKUP"

echo "Backup created: $BACKUP"

exit 0

