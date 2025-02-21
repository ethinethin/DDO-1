; stack.S - my attempt to implement a stack, stack pointer, and push/pop
;
; This puts the stack pointer into auto-increment register 31. It grows the
;   stack forward (i.e., new values come after old values), instead of the more
;   conventional backward growing. This uses <50 words, including the stack of
;   size 20, and can be safely stored on page 0.
;
; But.. it is a little limited. There is only room for 20 values on the stack
;   and there is no error checking (e.g. if you pop without first pushing). It
;   can be improved as necessary.


        START   100             ; store stack and related functions on page 0


; initstack - initialize stack and store at auto-inc reg 31
initstack,      .
        CLA
        TAD     stack
        DCA     31
        JMP     (initstack)
; stack size is 20 words
stack,          .
        PAD 20
; push - push contents of accumulator to the stack
push,           .
        DCA     (31)
        JMP     (push)
; pop - pop contents of stack to accumulator (additive)
pop,            .
        DCA     pop_tmp         ; store contents of accumulator
        ; store address of last stack entry
        TAD     31
        DCA     pop_addr
        ; subtract 1 from the stack position
        CMA                     ; load constant -1
        TAD     31              ;   add to current address at the stack pointer
        DCA     31              ;   store new stack pointer
        ; pull out value
        TAD     pop_tmp         ; reload accumulator
        TAD     (pop_addr)      ; pop stack value
        JMP     (pop)
pop_tmp,        0
pop_addr,       0
        PPD                     ; pad to page 1


; main - program entry point
main,
        JMS     initstack
        ; (var1 + var2) --> stack
        CLA
        TAD     var1
        TAD     var2
        JMS     push
        ; (var3 - var4) --> stack
        TAD     var4
        CMA IAC
        TAD     var3
        JMS     push
        ; (var5 + var6), then pop twice
        TAD     var5
        TAD     var6
        JMS     pop
        JMS     pop
        ; final result should be:
        ; (10 + 3) + (17 - 12) + (8 + 20) = 46 = 0000 0010 1110
        HLT
var1,           10
var2,           3
var3,           17
var4,           12
var5,           8
var6,           20