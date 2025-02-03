START 0                         ; start at address 0
mem_0, 2                        ; store value 2 at first byte
PAD 2047                        ; pad with 2047 0's to page 1
loop,                           ; start of page 1
CLA
TAD var1                        ; load var1
TAD mem_0                       ; add value at address 0 of page 0
DCA var1                        ; store this in var1
JMP page1_end                   ; jump to the end of page 1
var1, 0
PAD 2041                        ; pad until end of page 1 minus 1
page1_end,                      ; end of page 1 label
NOP                             ; no operation
CLA
TAD var2                        ; load variable in var2
HLT                             ; stop operation
var2, 100
END

; This is just demonstrating the ability to pad with 0s and to
; traverse pages. It's not perfect, but at least I can load
; values from page 0, and then move to page 2. I'd like to see
; if I can move from page 2 to page 1 using page 0 to store the
; address.. that will be in the next program.