CROSS= arm-unknown-eabi-

CC= $(CROSS)gcc
LD= $(CROSS)ld
OBJCOPY= $(CROSS)objcopy

STARTUP= startup.o
OBJS= main.o

# RaspberryPi library
OBJS+= rpi_lib/rpi_init.o rpi_lib/bss/clearbss.o rpi_lib/gpio/gpio.o rpi_lib/timer/timer.o rpi_lib/delay/delay.o
OBJS+= rpi_lib/serial/serial.o

# libgcc.a
LIBS = -L$(shell $(CC) -print-libgcc-file-name)
# newlib
LIBS += -L/home/jitomesky/cross/rpi/arm-unknown-eabi/arm-unknown-eabi/lib

CFLAGS = -mfloat-abi=soft -mlittle-endian
CFLAGS+= -fno-builtin
#CFLAGS+= -O2

########################
all:	rpi-micon.img

rpi-micon.elf:	$(STARTUP) $(OBJS)
	$(LD) -static -nostartup-T rpiMicon.lds $^ -o $@ $(LIBS) -lc -lgcc -lc

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
