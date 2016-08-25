CPUFLAGS=-march=armv7e-m -mcpu=cortex-m4 -mthumb
FPUFLAGS=-mfpu=fpv4-sp-d16 -mfloat-abi=hard

LD=arm-none-eabi-ld
LDFLAGS=

AS=arm-none-eabi-as
ASFLAGS=$(CPUFLAGS) $(FPUFLAGS)

OBJDUMP=arm-none-eabi-objdump
OBJDUMP_FLAGS=-Drth

GDB=arm-none-eabi-gdb
GDB_FLAGS=-x '.gdbinit'

OPENOCD=openocd
OPENOCD_FLAGS=-f openocd.cfg

.PHONY: all clean openocd gdb
all: nucleo.elf
clean:
	rm nucleo.elf nucleo.o

objdump: nucleo.elf
	$(OBJDUMP) $(OBJDUMP_FLAGS) nucleo.elf

openocd:
	$(OPENOCD) $(OPENOCD_FLAGS) -c "init; reset halt"

flash: nucleo.elf
	$(OPENOCD) $(OPENOCD_FLAGS) -c "init; reset halt; flash_elf $<"

gdb:
	$(GDB) $(GDB_FLAGS)

nucleo.elf: Makefile nucleo.ld nucleo.o
	$(LD) $(LDFLAGS) -T nucleo.ld -o $@ nucleo.o

nucleo.o: Makefile nucleo.s
	$(AS) $(ASFLAGS) -o $@ nucleo.s
