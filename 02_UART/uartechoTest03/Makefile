CROSS= arm-unknown-eabi-

CC= $(CROSS)gcc
LD= $(CROSS)ld
OBJCOPY= $(CROSS)objcopy

STARTUP= startup.o
OBJS= main.o

# RaspberryPi library
OBJS+= rpi_lib/rpi_init.o rpi_lib/bss/clearbss.o rpi_lib/gpio/gpio.o rpi_lib/timer/timer.o rpi_lib/delay/delay.o
OBJS+= rpi_lib/serial/serial.o

CFLAGS = -mfloat-abi=hard -mlittle-endian
CFLAGS+= -nostdinc -fno-builtin
#CFLAGS+= -O2

########################
all:	rpi-micon.img

rpi-micon.elf:	$(STARTUP) $(OBJS)
	$(LD) -static -nostdlib -T rpiMicon.lds $^ `$(CC) -print-libgcc-file-name` -o $@

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
