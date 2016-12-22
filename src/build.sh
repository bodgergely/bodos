export PATH=~/opt/cross/bin:$PATH
export BUILD_DIR=../build
export ISO_DIR=../isodir

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
i686-elf-as io.s -o $BUILD_DIR/io.o
i686-elf-as boot.s -o $BUILD_DIR/boot.o
i686-elf-gcc -c string.c -o $BUILD_DIR/string.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c terminal.c -o $BUILD_DIR/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c kernel.c -o $BUILD_DIR/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c cursor.c -o $BUILD_DIR/cursor.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c serialport.c -o $BUILD_DIR/serialport.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
#link the kernel
i686-elf-gcc -T linker.ld -o $BUILD_DIR/bodos.bin -ffreestanding -O2 -nostdlib $BUILD_DIR/boot.o $BUILD_DIR/string.o $BUILD_DIR/io.o $BUILD_DIR/cursor.o $BUILD_DIR/terminal.o $BUILD_DIR/kernel.o $BUILD_DIR/serialport.o -lgcc

#sanity check
if grub-file --is-x86-multiboot $BUILD_DIR/bodos.bin; then
    echo multiboot confirmed
else
    echo the file is not multiboot
fi


mkdir -p $ISO_DIR/boot/grub
cp $BUILD_DIR/bodos.bin $ISO_DIR/boot/
cp grub/grub.cfg $ISO_DIR/boot/grub/grub.cfg
grub-mkrescue -o $BUILD_DIR/bodos.iso $ISO_DIR
