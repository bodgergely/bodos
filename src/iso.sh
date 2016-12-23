#!/bin/sh
set -e
. ./build.sh

mkdir -p ../isodir
mkdir -p ../isodir/boot
mkdir -p ../isodir/boot/grub

cp ../sysroot/boot/bodos.kernel ../isodir/boot/bodos.kernel
cat > ../isodir/boot/grub/grub.cfg << EOF
menuentry "bodos" {
	multiboot /boot/bodos.kernel
}
EOF
grub-mkrescue -o ../bodos.iso ../isodir
