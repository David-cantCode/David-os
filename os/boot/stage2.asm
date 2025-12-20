[bits 16]
[org 0x7E00]  ;since we call at 0x2000:0

CODE_SEG            equ 0x08
DATA_SEG            equ 0x10
KERN_LOAD_PHYS      equ 0x00010000        ; 64 KiB
KERNEL_START_ADDR   equ 0x00100000        ;1 mib not mb lol
KERNEL_SECTORS      equ 186   ;kernel size, if os doesnt load fully just increase this lol, to much increase makes the os not load too 


stage2_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax 
    mov ss, ax
    mov sp, 0x7C00
    sti
    mov [BootDrive], dl 

    call clear_screen

    mov si, boot_info
    call print_string


    jmp vbe_done


vbe_setup:
    mov ax, 0x0000
    mov es, ax 
    mov di, 0x0500


    mov ax, 0x4F02
    mov bx, 0x11A
    or  bx, 0x4000    
    int 0x10


    ;check if success
    cmp ax, 0x004F
    jne vbe_error


    ; get mode info into ES:DI (AX=0x4F01, CX=mode)
    mov ax, 0x4F01
    mov cx, 0x0118
    int 0x10
    cmp ax, 0x004F
    jne vbe_error


    mov ax, 0x0000
    mov ds, ax
    mov si, 0x0500

    ;x res
    mov bx, [si + 0x12]
    mov ax, 0x0000
    mov es, ax
    mov di, 0x0404
    mov [di], bx
    mov word [di+2], 0      

    ; Yres
    mov bx, [si + 0x14]
    mov di, 0x0408
    mov [di], bx
    mov word [di+2], 0

    ; pitch
    mov bx, [si + 0x16]
    mov di, 0x040C
    mov [di], bx
    mov word [di+2], 0

    ; bytes per pixle
    mov al, [si + 0x19]
    mov di, 0x0410
    mov [di], ax     ;store low byte zero high bytes
    mov byte [di+1], 0
    mov byte [di+2], 0
    mov byte [di+3], 0


    mov di, 0x0400        
    mov al, [si + 0x28]
    mov [di], al
    mov al, [si + 0x29]
    mov [di+1], al
    mov al, [si + 0x2A]
    mov [di+2], al
    mov al, [si + 0x2B]
    mov [di+3], al

    ; store mode num a 0x0414 
    mov di, 0x0414
    mov ax, 0x0118
    mov [di], ax
    mov word [di+2], 0

    jmp vbe_done

vbe_error:
    ; clear the framebuffer info so kernel knows failure (add way to read it in future)
    mov ax, 0x0000
    mov es, ax
    mov di, 0x0400
    mov dword [di], 0
    mov dword [di+4], 0
    mov dword [di+8], 0
    mov dword [di+12], 0
    mov dword [di+16], 0
    mov dword [di+20], 0
    jmp vbe_done


vbe_done:
    jmp $














set_sectors:
    ;this function is called when there are less then 32 sectors to read


;read kernel to 0x10000
;make sure es points there

load_kernel_to_mem:
    ;how 0x13:0x02
    ;for all bios calls ah contains the type
    
    ;ah like always syscall type (0x02 for read) 
    ;al - num sectors to read 
    ;ch - cylindar
    ;cl starting sector
    ;dh head 
    ;dl drive number 
    ;bx points the location of disc -> sector
    ;for our loop New BX=Old BX+(Sectors Read×512)
    ;however bx cannot exceed 64kib 

    ;the kernel is then stored in the kernel phys to the disc
    ;however this can only read 64kib at a time (125) sectors
 

    ;idea loop to read 32 sectors at a time (31 for first cuz bios) 
    ;after sectors read head ++ and sectors needs to be read at 0x00 
    ;if head == 2 cylindar ++

    mov byte [sectors_left], KERNEL_SECTORS
    mov es, ax
    xor bx, bx  
.load_loop:

    mov ax, KERN_LOAD_PHYS >> 4    
    mov es, ax
    mov ah, 0x02
    mov al, [sectors_to_read]
    mov ch, [curr_cylinder]
    mov cl, [curr_sector]
    mov dh, [curr_head]
    mov dl, [BootDrive]
    int 0x13

    ;update pointers

    sub byte [sectors_left], al
    cmp [sectors_left], 0
    je kernel_load_done
    
    movzx cx, al ;mov sec count
    shl cx, 9 ;* 512
    add bx, cx ; back to new bx



    mov byte [sectors_to_read], 32
    
    mov byte [curr_sector], 1

    add byte [curr_head], 1
    cmp byte [curr_head], 2
    jl .load_loop

    ;if we need to increase cylindar

    mov byte [curr_head], 0
    add byte [curr_cylinder], 1
    jmp .load_loop


kernel_load_done:

    cli
    call enable_a20
    lgdt [gdt_descriptor]


    ;prep for protected mode
    mov eax, cr0
    or  eax, 1       
    mov cr0, eax
    jmp CODE_SEG:init_pm




enable_a20:
    ;allow the cpu to use mem > 1mb

    in   al, 0x92
    or   al, 2
    out  0x92, al
    ret


;**************************************
;***********START PROTECTED MODE*******
;**************************************

[BITS 32]
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax   
    mov gs, ax
    mov ss, ax 

    ;set stack pointer 
    mov esp, 0x90000
    

    ;copy kernel from PHYS -> ADDR
    mov esi, KERN_LOAD_PHYS        
    mov edi, KERNEL_START_ADDR    
    mov ecx, (KERNEL_SECTORS*512)/4 ;size in dwords
    rep movsd


    ;long jump 2 kernel
    jmp KERNEL_START_ADDR



print_string:
    mov ah, 0x0E
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

clear_screen:
    mov ah, 0x00    
    mov al, 0x03    
    int 0x10        
    ret

disk_read_error:
    jmp $




;**************************************
;**************DATA SECTION*************
;**************************************
gdt_start:
    dq 0x0000000000000000      
    dq 0x00CF9A000000FFFF         
    dq 0x00CF92000000FFFF          
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

BootDrive db 0

sectors_to_read db 28
sectors_left db 0
curr_sector db 0x04 ;stage1 - 1 sector, stage2 - 2 sectors
curr_head db 0
curr_cylinder db 0
curr_bx db 0


boot_info      db "DLOADER v0.5", 13, 10, 0

