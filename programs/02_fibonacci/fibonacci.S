;;;;
; fibonacci - generate 24th fibonacci number
;;;;
        START   2048            ; start at page 1
loop,
        CLA CLL
        TAD     var2            ; move var2 -> var1
        DCA     var1
        TAD     res             ; move res -> var2
        DCA     var2
        JMS     add             ; use add subroutine to add var1 + var2
        ISZ     index           ; increase loop index
        NOP                     ; placeholder to prevent skips (not necessary)
        TAD     num_loops
        CMA IAC                 ; find negative num_loops and compare to index
        TAD     index
        SZA                     ; skip next line if AC = 0 (index = num_loops)
        JMP     loop            ; loop if counter not 0
        CLA CLL                 ; finished loop, load the result into the AC
        TAD     res
        HLT                     ; done, stop operation
add,            0               ; add var1 and var2, store result in res
        CLA CLL
        TAD     var1
        TAD     var2
        DCA     res
        JMP     (add)           ; return from subroutine
var1,           0
var2,           0
res,            1
index,          0
num_loops,      23
        END
        