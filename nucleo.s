	.section .periph, "aw", %nobits
	.global RCC
RCC = 0x40023800
	.global GPIOC
GPIOC = 0x40020800

	.section .vector_table, "a", %progbits
	.global __vector_table
__vector_table:
        .word __stack_top
        .word __reset

	.section .stack, "aw", %nobits
__stack_bottom:
        .skip 0x1000
__stack_top:

        .section .text
        .thumb

	.global __reset
        .thumb_func
__reset:
        bl main
__end:
        wfi
        b __end
