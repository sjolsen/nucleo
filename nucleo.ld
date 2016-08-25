OUTPUT_ARCH(arm)
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")

MEMORY
{
        FLASH (RX) : ORIGIN = 0x08000000, LENGTH = 512K
        SRAM  (WX) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
        .vector_table : AT(0x08000000)
        {
                __vector_table = .;
                *(.vector_table)
                . = __vector_table + 0x1C4;
        } >FLASH =0x00000000

        .text : ALIGN(4)
        {
                *(.text)
        } >FLASH

        .stack (NOLOAD) : ALIGN(8)
        {
                *(.stack)
        } >SRAM
}
