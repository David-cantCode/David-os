

;*****************
;DLOADER 
;
;Date of start: AUG 12 2025
;Last Updated: DEC 20 2025
;*********************



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
bdb_sectors_per_fat:   dw 64       ;changed to 64 sectors per FAT

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

stage2_sectors equ 2
stage2_addr equ 0x0000
start:
;clear mem segs
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax 
    mov ss, ax
    mov sp, 0x7C00
    sti
    mov [BootDrive], dl ;preserve bios boot drive




load_stage2:
    mov ax, stage2_addr         ;stage2 load address segment
    mov es, ax

    mov bx, 0x7E00          
    mov ah, 0x02            ;bios 13h == sector read
    mov al, stage2_sectors  
    mov ch, 0               ;cylinder 0
    mov cl, 2               ;sector 2
    mov dh, 0               ;head 0
    mov dl, [BootDrive]    
    int 0x13
    jc disk_error


    jmp 0x0000:0x7E00



disk_error:
    jmp $



BootDrive db 0

times 510 - ($-$$) db 0 ;fill all bytes to 510 with 0s 
dw 0xAA55 ;fill last bytes 510-512 with special numbers so bios knows its bootable

