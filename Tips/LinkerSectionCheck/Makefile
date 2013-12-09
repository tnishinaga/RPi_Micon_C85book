CROSS= arm-unknown-eabi-

CC= $(CROSS)gcc
LD= $(CROSS)ld
OBJCOPY= $(CROSS)objcopy
OBJDUMP= $(CROSS)objdump
SIZE= $(CROSS)size
READELF= $(CROSS)readelf

STARTUP= startup.o
OBJS= main.o

# RaspberryPi library
OBJS+= rpi_lib/rpi_init.o rpi_lib/bss/clearbss.o

CFLAGS = -mfloat-abi=hard -mlittle-endian
CFLAGS+= -nostdinc -fno-builtin
CFLAGS+= -O0

########################
all:	rpi-micon.img

rpi-micon.elf:	$(STARTUP) $(OBJS)
	$(LD) -static -nostdlib -T rpiMicon.lds $^ `$(CC) -print-libgcc-file-name` -o $@
	$(OBJDUMP) -D rpi-micon.elf > rpi-micon.disas
	$(READELF) -a rpi-micon.elf > rpi-micon.readelf

.SUFFIXES: .elf .img

.elf.img:
	$(OBJCOPY) -O binary $< $@
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.S.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean::
	$(RM) -f *.o *.img *.elf */*.o */*/*.o 
	$(RM) -f tags *~

tags::
	ctags *.[chS]
