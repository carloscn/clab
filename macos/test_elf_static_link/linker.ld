ENTRY(main)
SECTIONS{
    . = 0x100000;
    start_of_text = .;
    .text : { a.o(.text) }
    end_of_text = .;
    . = 0x200000;
    .text : { b.o(.text) }
    . = 0x800000;
    .data : { *(.data) }
    . = 0x900000;
    .bss : { *(.bss) }
}
