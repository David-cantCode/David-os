ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
    /* Kernel load address */
    . = 0x00100000;            /* 1MB */

    /* Code */
    .text ALIGN(4096) :
    {
        *(.text)
    }

    /* Read-only data */
    .rodata ALIGN(4096) :
    {
        *(.rodata)
    }

    /* Initialized data */
    .data ALIGN(4096) :
    {
        *(.data)
    }

    /* Uninitialized data */
    .bss ALIGN(4096) :
    {
        *(COMMON)
        *(.bss)
    }

    /* ================================
       HEAP SECTION
       ================================ */

    . = ALIGN(4096);
    __heap_start = .;

    .heap ALIGN(4096) :
    {
        . += 0x01500000;       /* 21 MB heap */
    }

    __heap_end = .;


    /* ================================
       STACK SECTION
       ================================ */

    . = ALIGN(16);
    __stack_start = .;

    .stack ALIGN(16) :
    {
        . += 32K;
    }

    __stack_end = .;
}
