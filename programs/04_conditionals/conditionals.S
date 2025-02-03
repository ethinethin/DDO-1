START 2048
loop,
CLA CLL         ; clear AC and L
TAD var1        ; add addval to var1
TAD addval
DCA var1        ; deposit value in var1
SNL             ; skip on L != 0
JMP loop        ; loop if L = 0
CLA
TAD addval      ; double addval and re-store it
TAD addval
DCA addval
JMS check_if_done
JMP loop
check_if_done, 0
TAD maxval      ; load maxval, ones complement, and increment
CMA IAC
TAD addval      ; check if the two zero each other out
SZA             ; skip if AC = 0
JMP (check_if_done)
TAD addval      ; reload addval to display and halt
HLT
var1, 0
addval, 1
maxval, 1024
END