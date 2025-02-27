; interrupts.S - a test program for testing interrupts
;
; In a real program, one might want to set up device interrupt buffer locations
;   at the later auto-increment registers (e.g. 29, 30, and 31), so the earlier
;   registers are available for non-interrupt functions.
;
; Also, one might want to be more judicious in which interrupts are enabled. In
;   this program, the monitor and printer request interrupts even when there is
;   nothing to output. That will keep the program in the interrupt handling
;   section longer than necessary. For this program, there are approximately 8
;   to 9 interrupt-related instructions for every main program instruction.
;



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
        TAD     (buffer_0)
        DCA     16
        TAD     (buffer_0)
        DCA     17
        TAD     (buffer_0)
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
        ; store keystroke in buffer
        CLA
        KRB
        DCA     (16)
        ; mask buffer location
        TAD     16
        JMS     (bmask_0)
        DCA     16
        ; store string-terminating 0 in buffer
        DCA     (16)
        ; decrement buffer location by 1
        CMA
        TAD     16
        DCA     16
        JMP     (int_ttyk)

; int_ttyp - handle printer output
int_ttyp,       .
        ; load character from buffer
        CLA
        TAD     (17)
        ; if this is non-zero, print it
        SZA
        JMP     int_ttyp_p
        ; otherwise decrement buffer
        CMA
        TAD     17
        DCA     17
        JMP     (int_ttyp)
int_ttyp_p,
        ; output character
        TLS
        ; mask buffer location
        CLA
        TAD     17
        JMS     (bmask_0)
        DCA     17
        JMP     (int_ttyp)

; int_mon - handle monitor output
int_mon,        .
        ; load character from buffer
        CLA
        TAD     (18)
        ; if this is non-zero, print it
        SZA
        JMP     int_mon_p
        ; otherwise decrement buffer
        CMA
        TAD     18
        DCA     18
        JMP     (int_mon)
int_mon_p,
        ; output character
        VDC
        ; mask buffer location
        CLA
        TAD     18
        JMS     (bmask_0)
        DCA     18
        JMP     (int_mon)

; store page 2 addresses on page 0
buffer_0,       buffer
bmask_0,        bmask



; pad to page 1
        PPD
; main - main entry point for the program
main,
        ; set up interrupts
        CLA CLL
        JMS     int_setup
main_l,
        ; increment val1 and loop until val1 overflows
        CLA
        TAD     val1
        ISZ     val1
        JMP     main_l
        HLT
val1,   0



; pad to page 2
        PPD
; buffer location - pad with 32 zeroes
buffer,         .
        PAD     32
; buffer attributes
bmask_mask,     31
bmask_page,     buffer
; mask AC to fit in buffer
bmask,          .
        AND     bmask_mask
        TAD     bmask_page
        JMP     (bmask)



        END
