OUTPUT_ARCH(arm)
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")

MEMORY
{
        VFLASH (RX) : ORIGIN = 0x00000000, LENGTH = 512K
        LFLASH (RX) : ORIGIN = 0x08000000, LENGTH = 512K
        SRAM   (WX) : ORIGIN = 0x20000000, LENGTH = 128K
        PERIPH (RW) : ORIGIN = 0x40000000, LENGTH = 500M
}

SECTIONS
{
        .vector_table :
        {
                __vector_table = .;
                *(.vector_table)
                . = __vector_table + 0x1C4;
        } >VFLASH AT>LFLASH =0x00000000

        .text : ALIGN(4)
        {
                *(.text)
        } >VFLASH AT>LFLASH

        .stack (NOLOAD) : ALIGN(8)
        {
                __stack_bottom = .;
                . += 0x1000;
                __stack_top = .;
        } >SRAM

        .periph (NOLOAD) :
        {
                USART2 = 0x00004400;
                GPIOA  = 0x00020000;
                GPIOC  = 0x00020800;
                RCC    = 0x00023800;
                FLASH  = 0x00023C00;
        } >PERIPH
}
