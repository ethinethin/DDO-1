; stack2 - a better stack manager
;
; This implementation has some major changes from the last implementation. The
;   major change is it keeps track of the number of stack items to prevent
;   unchecked stack overflows or underflows. For now, the error functions for
;   those just halt the program, but more error handling could be added.
;
; A few other changes: more uniformity in subroutine and variable names with the
;   stack prefix; a single, shared tmp variable; pop function utilizing only one
;   tmp variable and just moving the stack pointer twice to allow popping and
;   resetting the stack pointer.
;

        START   100
stack,          .
        PAD     20
stack_n,        0
stack_max,      20
stack_tmp,      0

; set auto-inc reg 31 as the stack pointer
stack_init,     .
        CLA
        TAD     stack
        DCA     (31)
        JMP     (stack_init)

; push the current value onto the stack
stack_push,     .
        ; store accumulator
        DCA     stack_tmp
        ; check if within stack bounds
        TAD     stack_max
        CMA IAC
        TAD     stack_n
        ; if stack_n == stack_max, the stack will overflow
        SNA
        JMP     stack_overflow
        ; stack has room, add to it
        CLA
        TAD     stack_tmp
        DCA     (31)
        ; increase stack counter
        ISZ     stack_n
        NOP
        JMP     (stack_push)

; pop the last value from the stack
stack_pop,      .
        ; store accumulator
        DCA     stack_tmp
        ; ensure there is at least one value on the stack
        TAD     stack_n
        SNA
        JMP     stack_underflow
        ; stack has a value, move stack pointer back one so we can pop the value
        CLA CMA
        TAD     31
        DCA     31
        ; pop the value, add the previous accumulator, and store
        TAD     (31)
        TAD     stack_tmp
        DCA     stack_tmp
        ; move the stack pointer back one again
        CLA CMA
        TAD     31
        DCA     31
        ; reduce the stack counter by one
        CLA CMA
        TAD     stack_n
        DCA     stack_n
        ; return the previous accumulator plus popped value
        TAD     stack_tmp
        JMP     (stack_pop)

; stack error functions
stack_overflow,
stack_underflow,
        ; halt operation
        HLT

; pad to page 1
        PPD


; main program entry point
main,
        JMS     stack_init
        ; (var1 + var2) --> stack
        CLA
        TAD     var1
        TAD     var2
        JMS     stack_push
        ; (var3 - var4) --> stack
        TAD     var4
        CMA IAC
        TAD     var3
        JMS     stack_push
        ; (var5 + var6), then pop twice
        TAD     var5
        TAD     var6
        JMS     stack_pop
        JMS     stack_pop
        ; final result should be:
        ; (10 + 3) + (17 - 12) + (8 + 20) = 46 = 0000 0010 1110
        HLT
var1,           10
var2,           3
var3,           17
var4,           12
var5,           8
var6,           20