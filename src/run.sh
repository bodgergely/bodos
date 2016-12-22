export IMAGE_DIR=../build
./build.sh
mkdir -p ../log
bochs -f bochs/bochsrc.txt -q
#qemu-system-i386 -cdrom $IMAGE_DIR/bodos.iso