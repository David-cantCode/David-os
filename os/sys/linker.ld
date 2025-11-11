ENTRY(_start)
OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 0x0100000;
    .text : ALIGN(4096)
    {
        *(.text)
    }

    .rodata : ALIGN(4096)
    {
        *(.rodata)
    }

    .data : ALIGN(4096){
        *(.data)
    }

    .bss : ALIGN(4096)
    {
        *(COMMON)
        *(.bss)
    }

    
    /* Reserve x bytes for stack */
    . = ALIGN(16);
    __stack_start = .;
    .stack : ALIGN(16)
    {
        
        . += 32K; 
        /*bytes to allign*/
    }
    __stack_end = .;
}
