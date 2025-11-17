; AUG 13 2025

[BITS 32]

global _start 
extern kernel_ini
extern __stack_end 
section .text


extern __bss_start
extern __bss_end



_start:

    mov esp, __stack_end
    mov ebp, esp



    ;clear bss
    mov esi, __bss_start    
    mov edi, __bss_end     
    mov eax, 0

bss_clear_loop:
    cmp esi, edi
    jae bss_clear_done
    mov [esi], eax
    add esi, 4
    jmp bss_clear_loop

bss_clear_done:



.done:
    call kernel_ini
    jmp $                       



