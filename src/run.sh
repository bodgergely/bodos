export IMAGE_DIR=../build
./build.sh
qemu-system-i386 -cdrom $IMAGE_DIR/bodos.iso