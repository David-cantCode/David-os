; AUG 13 2025
; Intermediate 32-bit stage that clears VGA and writes a message before kernel.c

[BITS 32]

global _start 
extern kernel_main

section .text
_start:
   
   
    ;******************************
    ;************Clear screen******
    ;*******************************
    mov edi, 0xB8000           ; VGA text buffer base
    mov ecx, 80*25             ; # of cells
    mov ax, 0x0720             ; store the ascii code ' '


.clear_loop:
    stosw                      ; store ax and inc by 2  
    loop .clear_loop



    ;******************************
    ;******PRINT 'LOADING KERNEL'***
    ;*******************************
    mov edi, 0xB8000  ;point to first cell


    ;should change to a better loop
    mov al, 'L'
    mov ah, 0x07
    mov [edi], ax

    add edi, 2
    mov al, 'O'
    mov ah, 0x07
    mov [edi], ax

    add edi, 2
    mov al, 'A'
    mov ah, 0x07
    mov [edi], ax


    add edi, 2
    mov al, 'D'
    mov ah, 0x07
    mov [edi], ax


    add edi, 2
    mov al, 'I'
    mov ah, 0x07
    mov [edi], ax

    add edi, 2
    mov al, 'N'
    mov ah, 0x07
    mov [edi], ax


    add edi, 2
    mov al, 'G'
    mov ah, 0x07
    mov [edi], ax


    add edi, 2
    mov al, ' '
    mov ah, 0x07
    mov [edi], ax



    add edi, 2
    mov al, 'K'
    mov ah, 0x07
    mov [edi], ax



    add edi, 2
    mov al, 'E'
    mov ah, 0x07
    mov [edi], ax



    add edi, 2
    mov al, 'R'
    mov ah, 0x07
    mov [edi], ax


    add edi, 2
    mov al, 'N'
    mov ah, 0x07
    mov [edi], ax


    add edi, 2
    mov al, 'E'
    mov ah, 0x07
    mov [edi], ax
    

    add edi, 2
    mov al, 'L'
    mov ah, 0x07
    mov [edi], ax




 
mov esp, 0x90000

.done:
    call kernel_main
    jmp $                       


times 512 - ($ - $$) db 0
