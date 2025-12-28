ENTRY(_start)

SECTIONS
{
	. = 0x80000;

	text :
 	{
		KEEP(*(.text.boot))
 		*(.text .text.* .gnu.linkonce.t*)
	}

	.rodata :
	{
		*(.rodata .rodata.* .gnu.linkonce.r*)
	}
	
	PROVIDE(_data = .);
	.data : {
		*(.data .data.* .gnu.linkonce.d*)
	}

	.bss : {
			__bss_start = .;
			*(.bss)
			__bss_end = .;
	}
}
__bss_size = (__bss_end - __bss_start) >> 3;

