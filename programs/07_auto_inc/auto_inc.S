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
h,              72              ; ASCII 'H'
e,              101             ; ASCII 'e'
l1,             108             ; ASCII 'l'
l2,             108             ; ASCII 'l'
o1,             111             ; ASCII 'o'
comma,          44              ; ASCII ','
space,          32              ; ASCII -' '
w,              87              ; ASCII 'w'
o2,             111             ; ASCII 'o'
r,              82              ; ASCII 'r'
l3,             108             ; ASCII 'l'
d,              100             ; ASCII 'd'
newline,        10              ; ASCII '\n'
end,            0               ; string terminator
        END
