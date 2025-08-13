

; memmory segments
;CS code segment
;DS data segment
;ES extra seg
;SS stack
;GS and FS general purpose 

;Physical address = seg * 16 + offset



[BITS 16]
[ORG 0x7c00] ;tells assembler code will begin here


start: 

    cli ;disable interupts (making the cpu only focus here)
     
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    ;clearing segments
    mov sp, 0x7c00    ;stack pointer
   


    call enable_a20
    lgdt [gdt_descriptor]  ; Load GDT


    mov eax, cr0                                 
    or eax, 1              ; Set to 00001 (enable protected mode)
    mov cr0, eax


    jmp CODE_SEG:init_pm    



enable_a20: ;allows cpu to use memmory greater then 1mb 

    in al, 0x92 ; syscall id 
    or al, 2   
    out 0x92, al
    ret       
 




[bits 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax      ; Reload segment registers.
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000        

    ;call kernel_main        ; call kernerl

    cli ;disables interupts 
    hlt ;pauses cpu untill next interupt 



;___________________________________________
gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start



CODE_SEG equ 0x08 
DATA_SEG equ 0x10

times 510 - ($ - $$) db 0
dw 0xAA55 