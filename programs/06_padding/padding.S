;;;;
; padding - demonstrate padding zeroes with the PAD command, and
;           demonstrate moving linearly from one page to next
;;;;
        START   0               ; start at address 0
mem_0,          2               ; store value 2 at first byte
        PAD     2047            ; pad with 2047 0's to page 1
loop,                           ; start of page 1
        CLA
        TAD     var1            ; load var1
        TAD     mem_0           ; add value at address 0 of page 0
        DCA     var1            ; store this in var1
        JMP     page1_end       ; jump to the end of page 1
var1,           0
        PAD     2041            ; pad until end of page 1 minus 1
page1_end,                      ; end of page 1 label
        NOP                     ; no operation - "fall through" to page 2
        CLA
        TAD     var2            ; load variable in var2
        HLT                     ; stop operation
var2,           100
        END
