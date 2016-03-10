
OUTPUT_ARCH("mips")
OUTPUT_FORMAT("elf32-tradlittlemips")
ENTRY(__start)
SECTIONS {
	. = 0x100000;
	.text : {
		__text_begin__ = . ;
		*(.text);
		__text_end__ = . ;
	}
	.rodata : {
		__rodata_begin__ = . ;
		*(.rodata );
		__rodata_end__ = . ;
	}
	PROVIDE(__rodata_size__ = SIZEOF(.rodata ));
	.data : AT(__rodata_end__){
		__data_begin__ = . ;
		*(.data);
		__data_end__ = . ;
	}
	PROVIDE(__data_size__ = SIZEOF(.data));
	.bss : {
		__bss_begin__ = . ;
		*(.bss);
		__bss_end__ = . ;
	}
	PROVIDE(__bss_size__ = SIZEOF(.bss));
	/DISCARD/ : {
		*(.reginfo );
		*(.MIPS.abiflags );
	}
}
