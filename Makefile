CPUFLAGS=-march=armv7e-m -mcpu=cortex-m4 -mthumb
FPUFLAGS=-mfpu=fpv4-sp-d16 -mfloat-abi=hard

LD=arm-none-eabi-ld
LDFLAGS=

AS=arm-none-eabi-as
ASFLAGS=$(CPUFLAGS) $(FPUFLAGS)

CC=arm-none-eabi-gcc
CFLAGS=$(CPUFLAGS) $(FPUFLAGS) -ffreestanding -O2 -g -std=c11

OBJDUMP=arm-none-eabi-objdump
OBJDUMP_FLAGS=-Drth

GDB=arm-none-eabi-gdb
GDB_FLAGS=

OPENOCD=openocd
OPENOCD_FLAGS=-f openocd.cfg

C_HEADERS:=$(shell find -name '*.h')
C_SOURCES:=$(shell find -name '*.c')
S_SOURCES:=$(shell find -name '*.s')

BUILDDIR?=/tmp/nucleo-build

C_OBJS:=$(addprefix $(BUILDDIR)/, $(addsuffix .o, $(C_SOURCES)))
S_OBJS:=$(addprefix $(BUILDDIR)/, $(addsuffix .o, $(S_SOURCES)))

OBJS:=$(C_OBJS) $(S_OBJS)
ELF:=$(BUILDDIR)/nucleo.elf

.PHONY: all clean openocd gdb
all: $(ELF)

clean:
	rm -rf $(BUILDDIR)

objdump:
	$(OBJDUMP) $(OBJDUMP_FLAGS) $(ELF)

openocd:
	$(OPENOCD) $(OPENOCD_FLAGS) -c "init"

flash: $(ELF)
	$(OPENOCD) $(OPENOCD_FLAGS) -c "init; flash_elf $<; reset run; shutdown"

gdb:
	$(GDB) $(GDB_FLAGS)

$(ELF): Makefile nucleo.ld $(OBJS) $(DIRS)
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) -T nucleo.ld -o $@ $(OBJS)

$(C_OBJS):$(BUILDDIR)/%.c.o: %.c $(C_HEADERS) $(DIRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(S_OBJS):$(BUILDDIR)/%.s.o: %.s $(DIRS)
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<
