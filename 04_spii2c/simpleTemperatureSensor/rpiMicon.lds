OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x8000;

	.text : { *(.text*) }
	. = ALIGN(4);

	__rodata_start = .;
	.rodata : { *(.data*) }
	. = ALIGN(4);
	__rodata_end = .;

	__data_start = . ;
	.data : { *(.data*) }
	. = ALIGN(4);
	__data_end = . ;

	__bss_start = . ;
	.bss : { *(.bss*) }
	. = ALIGN(4);
	__bss_end = . ;
	. = ALIGN(4096);
	__free_start = .;
}