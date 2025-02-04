;;;;
; indirection - demonstrates some uses of indirection and
;               a "negative" loop index used for max loops
;;;;
        START   2048
        CLA CLL                 ; clear AC, L
        TAD     var2            ; deposit var2 in var1
        DCA     var1
        TAD     result          ; deposit result in var2
        DCA     var2
        JMS     add             ; call add subroutine
        TAD     result          ; load result to see it
        ISZ     counter         ; increase counter and break loop if 0
        JMP     (loop)
        HLT
add,            0               ; add var1 + var2, deposit in result
        CLA CLL
        TAD     var1
        TAD     var2
        DCA     result
        JMP     (add)           ; return to caller
var1,           0
var2,           0
result,         1
counter,        -23
loop,           2048
        END
