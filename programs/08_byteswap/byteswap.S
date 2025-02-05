;;;;
; byteswap - swap byte stored at var from 0000000011111111 to 1111111100000000,
;            swapping every 10 loops
;;;;
        START   2048
loop,
        CLA                     ; display var in AC indicator
        TAD     var
        ISZ     counter         ; increase counter
        NOP
        CLA
        TAD     max             ; compare counter to max
        CMA IAC
        TAD     counter
        SZA                     ; if they are equal, skip next line
        JMP     loop
        CLL                     ; clear link
        DCA     counter         ; reset counter
        TAD     var             ; byteswap the value in var
        BSW
        DCA     var
        JMP     loop            ; loop again
var,            255
counter,        0
max,            10
        END
