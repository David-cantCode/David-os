;AUG 12 2025

; memmory segments
;CS code segment
;DS data segment
;ES extra seg
;SS stack
;GS and FS general purpose 

;Physical address = seg * 16 + offset



;unfinished 

[BITS 16]
[ORG 0x7c00] ;tells assembler code will begin here





CODE_SEG equ 0x08 
DATA_SEG equ 0x10

KERNEL_LOAD_SEG equ 0x1000
KERNEL_START_ADDR equ 0x100000 






start: 

    cli ;disable interupts (making the cpu only focus here)
     
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    ;clearing segments
    mov sp, 0x7c00    ;stack pointer
    sti ;enable interupts


    ;LOAD KERNEL

    mov bx, KERNEL_LOAD_SEG
    mov dh, 0x00 
    mov dl, 0x80
    mov cl, 0x02
    mov ch, 0x00 
    mov ah, 0x02
    mov al, 8
    int 0x13

    jc disk_read_error ; added by chatgpt will fix later.


    cli
    call enable_a20
    lgdt [gdt_descriptor]  ; Load GDT


    mov eax, cr0                                 
    or eax, 1              ; Set to 00001 (enable protected mode)
    mov cr0, eax


    jmp CODE_SEG:init_pm    


    disk_read_error:
        cli





enable_a20: ;allows cpu to use memmory greater then 1mb 

    in al, 0x92 ; syscall id 
    or al, 2   
    out 0x92, al
    ret       
 




;**************************************
;***********START PROTECTED MODE*******
;**************************************
[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax      ; Reload segment registers.
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000        


    jmp CODE_SEG:KERNEL_START_ADDR ;fix? 





;**************************************
;**************CREATE GPT TABLE********

gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start





times 510 - ($-$$) db 0
dw 0xAA55 ;finish last 2 bytes with special number to let bios know its bootable
