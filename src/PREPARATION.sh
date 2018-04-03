sudo apt-get install bochs
sudo apt-get install bochs-x
sudo apt-get install bochs-sdl
sudo apt-get install qemu
sudo apt-get install xorriso
sudo apt-get install gcc-multilib  # required for building 32 bit gcc compiler

#Download binutils source(2.29.1) and gcc source (7.2.0)

#First build binutils:
export HOME=$1
export BINUTILS_DIR=$HOME/binutils
export GCC_DIR=$HOME/gcc
export PREFIX="$HOME/opt/gcc-7.2.0"

mkdir -p $BINUTILS_DIR
mkdir -p $GCC_DIR

cd $BINUTILS_DIR
mkdir build-binutils-2.29.1
cd build-binutils-2.29.1
../binutils-2.29.1/configure --prefix="$PREFIX" --disable-nls --disable-werror
make -j8
make install

#Build native(not the cross compiler) GCC:
cd $GCC_DIR/gcc-7.2.0
./contrib/download_prerequisites
mkdir ../build-gcc-7.2.0
cd ../build-gcc-7.2.0
../gcc-7.2.0/configure --prefix="$PREFIX" --disable-nls --enable-languages=c,c++
make -j8
make install

#Test the version:
$HOME/opt/gcc-7.2.0/bin/gcc --version

#Add it to the path:
export PATH="$HOME/opt/gcc-7.2.0/bin:$PATH"


#Build the Cross binutils and GCC Cross compiler:

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

#First we need to build binutils with the new compiler:
cd $BINUTILS_DIR
mkdir build-cross-binutils-2.29.1-gcc.7.2.0
cd build-cross-binutils-2.29.1-gcc.7.2.0
../binutils-2.29.1/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j8
make install

#Now build the cross GCC:
cd $GCC_DIR
 
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
 
mkdir build-cross-gcc-7.2.0
cd build-cross-gcc-7.2.0
../gcc-7.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make -j8 all-gcc
make -j8 all-target-libgcc
make install-gcc
make install-target-libgcc
