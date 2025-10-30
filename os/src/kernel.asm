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
   

 
    mov esp, __stack_end
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


