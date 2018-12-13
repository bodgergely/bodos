DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)
 
CPPFLAGS=-O2 -g -fno-exceptions -fno-rtti
LDFLAGS=
LIBS=
 
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include
 
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude -ffreestanding -Wall -Wextra
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc
 
ARCHDIR=arch/$(HOSTARCH)
 
include $(ARCHDIR)/make.config
 
ARCH_FLAGS:=-Di386
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
 
KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \
kernel/kprintf.o \
kernel/debug.o \
kernel/mem/allocator.o \
kernel/mem/kmalloc.o \
kernel/sched/sched.o \
kernel/proc/proc.o \
kernel/utils/random.o \
kernel/utils/hash.o \
kernel/tests/RandomTest.o \
kernel/tests/TestCenter.o \
 
OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
 
LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
 
.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .cpp .
 
all: bodos.kernel
 
bodos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CPPFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot bodos.kernel
 
$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:

	OBJ=`$(CC) $(CPPFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@
 

.c.o:
	$(CC) -MD -c $< -o $@ -std=c++14 $(CPPFLAGS) $(ARCH_FLAGS)
	
.cpp.o:
	$(CC) -MD -c $< -o $@ -std=c++14 $(CPPFLAGS) $(ARCH_FLAGS)
 
.s.o:
	$(CC) -MD -c $< -o $@ $(CPPFLAGS) $(ARCH_FLAGS)
 
clean:
	rm -f bodos.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
 
install: install-headers install-kernel
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-kernel: bodos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp bodos.kernel $(DESTDIR)$(BOOTDIR)
 
-include $(OBJS:.o=.d)

