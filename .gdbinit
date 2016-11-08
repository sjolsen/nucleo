define flash
  monitor flash_elf /tmp/nucleo-build/nucleo.elf
end

target remote localhost:3333
symbol-file /tmp/nucleo-build/nucleo.elf
