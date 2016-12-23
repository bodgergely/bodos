#!/bin/sh
set -e
. ./iso.sh

bochs -f bochs/bochsrc.txt -q