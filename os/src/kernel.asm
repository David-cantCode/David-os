; AUG 13 2025
; Intermediate 32-bit stage that clears VGA and writes a message before kernel.c

[BITS 32]

global _start 
extern kernel_ini
extern __stack_end 
section .text

extern fb_addr
extern pitch


_start:
   
<<<<<<< Updated upstream
   
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




     mov esp, __stack_end
=======
 
    mov esp, __stack_end
>>>>>>> Stashed changes
    mov ebp, esp




    ; === Load VBE info that bootloader left in memory ===
    mov eax, [0x0400]     ; framebuffer physical address
    mov [fb_addr], eax

    mov eax, [0x040C]     ; bytes per scanline (pitch)
    mov [pitch], eax

    ; === Draw one pixel (100, 100) white ===
    mov eax, 0x00FFFFFF   ; color = white (ARGB)
    mov ebx, 100          ; x
    mov ecx, 100          ; y
    call draw_pixel


.done:
    call kernel_ini
    jmp $                       


draw_pixel:
    push edx
    push edi

    mov edx, [pitch]          ; bytes per scanline
    mov edi, ecx              ; y
    imul edi, edx             ; y * pitch
    mov edx, ebx              ; x
    shl edx, 2                ; x * 4 (32bpp)
    add edi, edx
    add edi, [fb_addr]        ; + framebuffer base

    mov [edi], eax            ; write 32-bit color

    pop edi
    pop edx
    ret


