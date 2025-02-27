; interrupts.S - a test program for testing interrupts
;
; Interrupts aren't ready yet in the simulator but when they are, this program
;   will be ready to test them!

; start at page 0, mem 0
        START   0
mem_0,          .
        ; store AC and L
        DCA     int_AC
        RAL
        DCA     int_L
        ; go to interrupt skip chain
        JMS     int_skipchain
        ; reload AC and L
        CLA CLL
        TAD     int_L
        RAR
        TAD     int_AC
        ; re-enable interrupts and return
        ION
        JMP     (mem_0)
int_AC,         0
int_L,          0

; skip some memory to leave room for auto-inc registers
        PAD     24

; int_setup - setup interrupts for all hardware
int_setup,      .
        ; set up keyboard, printer, and monitor buffer pointers
        CLA
        TAD     buffer_0
        DCA     16
        TAD     buffer_0
        DCA     17
        TAD     buffer_0
        DCA     18
        ; enable device-specific interrupt flags
        IAC
        KIE
        TIE
        VIE
        ; enable system-wide interrupt flag
        ION
        JMP     (int_setup)

; int_skipchain - cycle through all devices and check which are ready
int_skipchain,  .
        ; check keyboard
        KSF
        SKP
        JMS     int_ttyk
        ; check printer
        TSF
        SKP
        JMS     int_ttyp
        ; check monitor
        VSF
        SKP
        JMS     int_mon
        JMP     (int_skipchain)

; int_ttyk - handle keyboard input
int_ttyk,       .
        ; load keystroke into buffer
        CLA
        TAD     16              ; load buffer location
        JMS     mask_b          ;   ensure in buffer
        DCA     16              ;   store buffer location
        KRB                     ; load keystroke
        DCA     (16)            ;   store in buffer
        ; add a string-terminating 0 after it
        TAD     16
        JMS     mask_b
        DCA     16
        DCA     (16)
        ; move buffer back one, before terminator
        CMA
        TAD     16
        JMS     mask_b
        DCA     16
        JMP     (int_ttyk)

; int_ttyp - handle printer output
int_ttyp,       .
        ; load current character
        CLA
        TAD     17
        JMS     mask_b
        DCA     17
        TAD     (17)
        ; if it's 0, move the buffer back and leave
        SZA
        JMP     int_ttyp_p
        CMA
        TAD     17
        JMS     mask_b
        DCA     17
        JMP     (int_ttyp)
int_ttyp_p,
        ; print out character
        TLS
        JMP     (int_ttyp)

; int_mon - handle monitor output
int_mon,        .
        ; load current character
        CLA
        TAD     18
        JMS     mask_b
        DCA     18
        TAD     (18)
        ; if it's 0, move the buffer back and leave
        SZA
        JMP     int_mon_p
        CMA
        TAD     18
        JMS     mask_b
        DCA     18
        JMP     (int_mon)
int_mon_p,
        ; print out character
        VDC
        JMP     (int_mon)

; mask_b - wrap address to fit within the circular buffer
mask_b,         .
        ; mask the address to ensure it's within the buffer size
        AND     (buffer_mask_0)
        ; add the page address to ensure it's in the correct page
        TAD     (buffer_page_0)
        JMP     (mask_b)

; store page 2 variable addresses on page 0
buffer_0,       buffer
buffer_mask_0,  buffer_mask
buffer_page_0,  buffer_page



; pad to page 1
        PPD
; main - main entry point for the program
main,
        CLA CLL
        ; set up interrupts
        JMS     int_setup
main_l,
        ; loop eternally
        NOP
        JMP     main_l



; pad to page 2
        PPD
; buffer location - pad with 8 zeroes
buffer,         .
        PAD     8
; buffer attributes
buffer_mask,    7
buffer_page,    4096
        END
