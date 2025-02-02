START 2048                      ; start at page 1, memory 0
LOOP,
CLA                             ; put VAR2 -> VAR1
TAD VAR2
DCA VAR1
TAD RES                         ; put RES -> VAR2
DCA VAR2
JMP ADD                         ; go add VAR1 + VAR2
RETURN,
ISZ COUNTER                     ; increase COUNTER
JMP LOOP                        ; loop if COUNTER not 0
HLT
ADD,                            ; add VAR1 and VAR2, store result in RES
CLA
TAD VAR1
TAD VAR2
DCA RES
JMP RETURN
VAR1, 0
VAR2, 0
RES, 1
COUNTER, -1000