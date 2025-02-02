START 2048                      ; start at page 1, memory 0
CLA CLL                         ; initially clear accumulator and link bit
LOOP,
TAD VAR2                        ; put VAR2 -> VAR1
DCA VAR1
TAD RES                         ; put RES -> VAR2
DCA VAR2
JMP ADD                         ; add VAR1 + VAR2
RETURN,
ISZ COUNTER                     ; increase COUNTER
JMP LOOP                        ; loop if COUNTER not 0
CLA                             ; finished loop, load the result into the AC
TAD RES
HLT                             ; stop operation
ADD,                            ; add VAR1 and VAR2, store result in RES
TAD VAR1
TAD VAR2
DCA RES
JMP RETURN
VAR1, 0
VAR2, 0
RES, 1
COUNTER, -23