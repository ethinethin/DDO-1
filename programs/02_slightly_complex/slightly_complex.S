START 2048                      ; start at page 1, memory 0
CLA CLL                         ; initially clear accumulator and link bit
loop,
TAD var2                        ; put var2 -> var1
DCA var1
TAD res                         ; put res -> var2
DCA var2
JMP add                         ; add var1 + var2
return,
ISZ counter                     ; increase counter
JMP loop                        ; loop if counter not 0
CLA                             ; finished loop, load the result into the AC
TAD res
HLT                             ; stop operation
add,                            ; add var1 and var2, store result in res
TAD var1
TAD var2
DCA res
JMP return
var1, 0
var2, 0
res, 1
counter, -23