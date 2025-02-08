;;;;
; auto_inc - this program outputs "Hello, world!" by using an auto-increment
;            register; if these registers (memory locations 16-31) are used via
;            indirection, the address stored will automatically increment prior
;            to execution of the instruction
;;;;
        START   2048
        CLA
        TAD     hello           ; store address in hello in auto-increment
                                ; register
        DCA     16
loop,
        TAD     (16)            ; address in 16 will auto-increment before this
                                ; operation
        SNA                     ; skip if accumulator != 0, otherwise end
        JMP     finished
        JMS     ttyo            ; send to ttyo subroutine and loop
        JMP     loop
finished,
        CLA
        TAD     hello
        DCA     16
        JMP     loop
ttyo,           0               ; output to TTY device - need to implement in
                                ; simulator and assembler - so this is currently
                                ; a placeholder subroutine
        CLA
        JMP     (ttyo)
hello,          .
        72 101 108 108          ; ASCII: 'H' 'e' 'l' 'l'
        111 44 32 87            ; ASCII: 'o' ',' ' ' 'W'
        111 114 108 100         ; ASCII: 'o' 'r' 'l' 'd'
        33 10 0                 ; ASCII: '!' '\n' NUL
        END
