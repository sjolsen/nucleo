define flash
  monitor flash_elf nucleo.elf
end

target remote localhost:3333
symbol-file nucleo.elf
