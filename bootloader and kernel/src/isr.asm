
global isr_stub_table    ; make visible for c
global irq1_handler  
extern keyboard_callback ; C function we'll call from here
extern PIC_sendEOI       ; PIC End Of Interrupt function
extern irq_handler_c

;*************************
;*****STUB MACROS********
;*************************


;works and gets called when error
%macro isr_err_stub 1
isr_stub_%+%1:
   ; call exception_handler
    cli
    pushad
    popad
    add esp, 4       ; skip error code 
    sti
    iret
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
    iret
%endmacro


;*************************
;******STUBS*************
;*************************
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




;counter to assign stubs in memmory


global isr_stub_table
isr_stub_table:
%assign i 0         
%rep 32
    dd isr_stub_%+i
%assign i i+1
%endrep


;*************************
;******IRQ1 HANDLER*******
;*************************
irq1_handler:
 
    cli                 ; disable interrupts
    pushad              ; save EAX..EDI (32-bit)
    pushfd              ; save EFLAGS


    push dword 1        ; push IRQ number as 32-bit cdecl argument
    call irq_handler_c  ; common handler, will call registered C handler
    add  esp, 4         ; clean the argument stack

    popfd               ; restore EFLAGS
    popad               ; restore registers

    ; send EOI to PICs (do this after C handler so handler can use ports)
    push dword 1
    call PIC_sendEOI
    add  esp, 4

    sti                 ; optionally, but usually not needed before iret
    iretd               ; return from interrupt

