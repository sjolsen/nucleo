CPUFLAGS=-march=armv7e-m -mcpu=cortex-m4 -mthumb
FPUFLAGS=-mfpu=fpv4-sp-d16 -mfloat-abi=hard

LD=arm-none-eabi-ld
LDFLAGS=

AS=arm-none-eabi-as
ASFLAGS=$(CPUFLAGS) $(FPUFLAGS)

CC=arm-none-eabi-gcc
CFLAGS=$(CPUFLAGS) $(FPUFLAGS) -ffreestanding -O2 -g -std=c11 -Wall -Wextra -Werror -Istdlib -I.

CXX=arm-none-eabi-g++
CXXFLAGS=$(CPUFLAGS) $(FPUFLAGS) -ffreestanding -O2 -g -std=c++11 -Wall -Wextra -Werror -Istdlib -I. -fno-exceptions

OBJDUMP=arm-none-eabi-objdump
OBJDUMP_FLAGS=-Drth

GDB=arm-none-eabi-gdb
GDB_FLAGS=

OPENOCD=openocd
OPENOCD_FLAGS=-f openocd.cfg

HEADERS:=$(shell find -name '*.h' -o -name '*.hh')
C_SOURCES:=$(shell find -path ./stdlib -prune -o -name '*.c' -print)
STDLIB_SOURCES:=$(shell find ./stdlib -name '*.c')
CC_SOURCES:=$(shell find -name '*.cc')
S_SOURCES:=$(shell find -name '*.s')

BUILDDIR?=/tmp/nucleo-build

C_OBJS:=$(addprefix $(BUILDDIR)/, $(addsuffix .o, $(C_SOURCES)))
STDLIB_OBJS:=$(addprefix $(BUILDDIR)/, $(addsuffix .o, $(STDLIB_SOURCES)))
CC_OBJS:=$(addprefix $(BUILDDIR)/, $(addsuffix .o, $(CC_SOURCES)))
S_OBJS:=$(addprefix $(BUILDDIR)/, $(addsuffix .o, $(S_SOURCES)))

OBJS:=$(C_OBJS) $(STDLIB_OBJS) $(CC_OBJS) $(S_OBJS)
ELF:=$(BUILDDIR)/nucleo.elf

OBJDUMP_TARGET?=$(ELF)

.PHONY: all clean openocd gdb
all: $(ELF)

clean:
	rm -rf $(BUILDDIR)

objdump:
	$(OBJDUMP) $(OBJDUMP_FLAGS) $(OBJDUMP_TARGET)

openocd:
	$(OPENOCD) $(OPENOCD_FLAGS) -c "init"

flash: $(ELF)
	$(OPENOCD) $(OPENOCD_FLAGS) -c "init; flash_elf $<; reset run; shutdown"

gdb:
	$(GDB) $(GDB_FLAGS)

$(ELF): nucleo.ld $(OBJS) $(DIRS) Makefile
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) -T nucleo.ld -o $@ $(OBJS)

$(C_OBJS):$(BUILDDIR)/%.c.o: %.c $(HEADERS) $(DIRS) Makefile
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(STDLIB_OBJS):$(BUILDDIR)/%.c.o: %.c $(HEADERS) $(DIRS) Makefile
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -nostdlib -c -o $@ $<

$(CC_OBJS):$(BUILDDIR)/%.cc.o: %.cc $(HEADERS) $(DIRS) Makefile
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(S_OBJS):$(BUILDDIR)/%.s.o: %.s $(DIRS) Makefile
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<
