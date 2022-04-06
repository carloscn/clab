SECTIONS
{
	. = 0x80000,
    _stext_boot = .;
	.text.boot : { *(.text.boot) }
    _etext_boot = .;

	.text : AT(0x90000) { *(.text) }

    	.rodata : { *(.rodata) }
	.data : { *(.data) }
	. = ALIGN(0x8);
	bss_begin = .;
	.bss : { *(.bss*) } 
	bss_end = .;
}
