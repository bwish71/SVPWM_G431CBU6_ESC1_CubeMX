#!/bin/bash

SERIAL="2097368D4B421"
PATTERN="/dev/cu.usbmodem*${SERIAL}"
echo "Debug: Looking for device with pattern: $PATTERN"

DEV=$(ls $PATTERN 2>/dev/null | head -n 1)
echo "Debug: Result of ls: '$DEV'"

LINK="/dev/stm32"

if [ -n "$DEV" ]; then
    echo "Debug: Device found: $DEV"
    if [ -L "$LINK" ]; then
        echo "Debug: Removing old symlink $LINK"
        rm "$LINK"
    fi
    ln -s "$DEV" "$LINK"
    echo "Symlink created: $LINK -> $DEV"
else
    echo "STM32 device not found. Check if the device is connected and the serial number is correct."
    ls /dev/cu.usbmodem*
fi
