	.section .vector_table, "a", %progbits
	.global __vector_table
__vector_table:
        .word __stack_top
        .word __reset
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler
	.word __default_handler

	.section .stack, "aw", %nobits
__stack_bottom:
        .skip 0x1000
__stack_top:

        .section .text
        .thumb

        .global __end
__end:

        b __end

	.global __reset
        .thumb_func
__reset:
        bl main
        b __end

	.thumb_func
__default_handler:
        b __end
