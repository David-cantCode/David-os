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
bdb_sectors_per_fat:   dw 64         ; <- changed to 64 sectors per FAT

bdb_sectors_per_track: dw 32
bdb_heads:             dw 2
bdb_hidden_sectors:    dd 0
bdb_large_sector_count: dd 65536     ; <- set to 65536 sectors = 32 MiB

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
KERNEL_SECTORS      equ 17        ; kernel size 

start:

;----clear memmory segments--------
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax 
    mov ss, ax
    mov sp, 0x7C00
    sti

    mov [BootDrive], dl            ; preserve bios boot drive

; --- read kernel to 0x10000 (ES:BX must point there) ---

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
    or  eax, 1       ;Enable protected mode (set to 00001)
    mov cr0, eax
    jmp CODE_SEG:init_pm


;----Allow cpu to use memmory > 1MB 

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
    

; --- Copy kernel from 0x00010000 -> 0x00100000 ---
    mov esi, KERN_LOAD_PHYS        ; src
    mov edi, KERNEL_START_ADDR     ; dst
    mov ecx, (KERNEL_SECTORS*512)/4 ; dwords to copy
    rep movsd

; --- Enter kernel ---
    jmp KERNEL_START_ADDR


; --- Made by chatgpt im not complaing cuz it works--
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

; dw 0x55AA           ;changed from 0xAA55 -> for iso 
dw 0xAA55 ;fill last bytes 510-512 with special numbers so bios knows its bootable
