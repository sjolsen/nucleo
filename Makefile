CPUFLAGS=-march=armv7e-m -mcpu=cortex-m4 -mthumb
FPUFLAGS=-mfpu=fpv4-sp-d16 -mfloat-abi=hard

LD=arm-none-eabi-ld
LDFLAGS=

AS=arm-none-eabi-as
ASFLAGS=$(CPUFLAGS) $(FPUFLAGS)

CC=arm-none-eabi-gcc
CFLAGS=$(CPUFLAGS) $(FPUFLAGS) -ffreestanding -O2 -g

OBJDUMP=arm-none-eabi-objdump
OBJDUMP_FLAGS=-Drth

GDB=arm-none-eabi-gdb
GDB_FLAGS=

OPENOCD=openocd
OPENOCD_FLAGS=-f openocd.cfg

.PHONY: all clean openocd gdb
all: nucleo.elf
clean:
	rm nucleo.elf nucleo.o main.o

objdump: nucleo.elf
	$(OBJDUMP) $(OBJDUMP_FLAGS) nucleo.elf

openocd:
	$(OPENOCD) $(OPENOCD_FLAGS) -c "init"

flash: nucleo.elf
	$(OPENOCD) $(OPENOCD_FLAGS) -c "init; flash_elf $<; reset run; shutdown"

gdb:
	$(GDB) $(GDB_FLAGS)

nucleo.elf: Makefile nucleo.ld nucleo.o main.o
	$(LD) $(LDFLAGS) -T nucleo.ld -o $@ nucleo.o main.o

nucleo.o: Makefile nucleo.s
	$(AS) $(ASFLAGS) -o $@ nucleo.s

main.o: Makefile main.c
	$(CC) $(CFLAGS) -c -o $@ main.c
