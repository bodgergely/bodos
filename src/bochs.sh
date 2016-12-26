#!/bin/sh
set -e
mkdir -p ../log
. ./iso.sh

bochs -f bochs/bochsrc.txt -q
