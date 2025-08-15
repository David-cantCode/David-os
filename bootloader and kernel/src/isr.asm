[BITS 32]
global isr_stub_table    ; make the table visible to C
extern exception_handler ; tell NASM that exception_handler is in C

;=============================
; Macros for stubs
;=============================
%macro isr_no_err_stub 1
isr_stub_%+%1:
    pusha                  ; save registers
    call exception_handler
    popa
    iret
%endmacro

%macro isr_err_stub 1
isr_stub_%+%1:
    pusha
    call exception_handler
    popa
    iret
%endmacro


;=============================
; Define stubs for exceptions 0–31
;=============================
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

;*****************************
; Create table of all ISR addresses
;**************************


;display debug letter L (doesnt fucking work)
    mov edi, 0xB8000 
    mov al, 'L'
    mov ah, 0x07
    mov [edi], ax



section .data

isr_stub_table:
%assign i 0
%rep 32
    dd isr_stub_%+i
%assign i i+1
%endrep
