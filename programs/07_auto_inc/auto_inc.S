;;;;
; auto_inc - this program outputs "Hello, world!" by using an auto-increment
;            register; if these registers (memory locations 16-31) are used via
;            indirection, the address stored will automatically increment prior
;            to execution of the instruction
;;;;
        START   2048
        CLA
        TAD     hello           ; store address of hello in auto-increment
        DCA     16              ;   register at memory location 16
loop,
        TAD     (16)            ; address in 16 will auto-increment before this
                                ;   operation
        SNA                     ; skip if accumulator != 0, otherwise end
        JMP     reset           ; reset string position when finished outputting
        JMS     ttyo            ; send to ttyo subroutine and loop
        JMP     loop
reset,
        CLA                     ; re-store address of hello in auto-increment
        TAD     hello           ;   register at memory location 16
        DCA     16
        JMP     loop            ; re-enter loop
ttyo,           0               ; output to TTY printer - wait until device is
ttyo_l,                         ;   ready, then print out character and clear AC
        TSF
        JMP     ttyo_l
        TLS
        CLA
        JMP     (ttyo)          ; return from subroutine
hello,          .
        72 101 108 108          ; ASCII: 'H' 'e' 'l' 'l'
        111 44 32 87            ; ASCII: 'o' ',' ' ' 'W'
        111 114 108 100         ; ASCII: 'o' 'r' 'l' 'd'
        33 10 0                 ; ASCII: '!' '\n' NUL
        END
