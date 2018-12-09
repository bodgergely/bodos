#!/bin/bash
# of=</dev/usb_device_number>
# eg. of=/dev/sda1
if [ $1 ]
then
    sudo dd bs=4M if=../bodos.iso of=$1 conv=fdatasync
else
    echo "Usage: <path-to-usb-device-file>"
    echo "e.g. /dev/sda1" 
fi