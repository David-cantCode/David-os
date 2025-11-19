;AUG 12 2025
;DAVID BOOTLOADER aug 12

; memmory segments
;CS code segment
;DS data segment
;ES extra seg
;SS stack
;GS and FS general purpose 

;Physical address = seg * 16 + offset




;cli - disable interupts
; sti - enable interupts






[BITS 16]
[ORG 0x7C00]


;********************************
;FAT 16 BIOS PARAMERTER BLOCK**
;*********************************
jmp short start
nop
bdb_oem                db 'MSWIN4.1'
bdb_bytes_per_sector:  dw 512
bdb_sectors_per_cluster: db 4
bdb_reserved_sectors:  dw 4
bdb_fat_count:         db 2
bdb_dir_entries_count: dw 512

bdb_total_sectors:     dw 0          ; keep 0 if using bdb_large_sector_count
bdb_media_descriptor_type: db 0F8h
bdb_sectors_per_fat:   dw 64         ;changed to 64 sectors per FAT

bdb_sectors_per_track: dw 32
bdb_heads:             dw 2
bdb_hidden_sectors:    dd 0
bdb_large_sector_count: dd 65536     ;32 MiB

;extent boot record 
ebr_drive_number: db 0 
db 0 ;reserved
ebr_signature: db 29h
ebr_volume_id: db 12h, 34h, 56h, 78h
ebr_volume_label: db 'DAVID OS   ' ;keep this size (11 bytes)
ebr_system_id: db 'FAT16   ' ; 8 bytes

CODE_SEG            equ 0x08
DATA_SEG            equ 0x10

KERN_LOAD_PHYS      equ 0x00010000        ; 64 KiB
KERNEL_START_ADDR   equ 0x00100000        ; 1 mib not mb lol

KERNEL_SECTORS      equ 82     ; kernel size, if os doesnt load fully just increase this lol, to much increase makes the os not load too 


start:

;clear mem segs
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax 
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov [BootDrive], dl            ; preserve bios boot drive


;read kernel to 0x10000


    ;setup vbe framebuffer


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



;read kernel to 0x10000
;make sure es points there



    mov ax, KERN_LOAD_PHYS >> 4    ; ES = 0x1000
    mov es, ax
    xor bx, bx                     ; BX = 0
    mov ah, 0x02                   ; BIOS read sectors (CHS)
    mov al, KERNEL_SECTORS         ; count
    mov ch, 0x00                   ; cylinder 0
    mov cl, 0x02                   ; sector 2 (sector 1 is boot)
    mov dh, 0x00                   ; head 0
    mov dl, [BootDrive]            ; correct drive
    int 0x13
    jc disk_read_error




    cli
    call enable_a20
    lgdt [gdt_descriptor]

    mov eax, cr0
    or  eax, 1       ;enable protected mode 
    mov cr0, eax
    jmp CODE_SEG:init_pm




enable_a20:
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
    mov fs, ax   ; Reload segment registers.
    mov gs, ax
    mov ss, ax 
    mov esp, 0x90000
    

;copy kernel from 0x00010000 -> 0x00100000 
    mov esi, KERN_LOAD_PHYS        ; src
    mov edi, KERNEL_START_ADDR     ; dst
    mov ecx, (KERNEL_SECTORS*512)/4 ;size in dwords
    rep movsd

;ent kernel
    jmp KERNEL_START_ADDR


;made by chatgpt im not complaing cuz it works
disk_read_error:
    
    cli
.hang: hlt
      jmp .hang




;**************************************
;**************CREATE GPT TABLE********
;**************************************



gdt_start:
    dq 0x0000000000000000          ;confusing read about segement descriptors in the future
    dq 0x00CF9A000000FFFF         
    dq 0x00CF92000000FFFF          
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

BootDrive db 0

times 510 - ($-$$) db 0 ;fill all bytes to 510 with 0s 
dw 0xAA55 ;fill last bytes 510-512 with special numbers so bios knows its bootable