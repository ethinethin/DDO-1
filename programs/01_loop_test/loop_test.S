START 2048              ; Start at 2048
LOOP,
CML                     ; complement the link bit
CLA                     ; clear the accumulator
TAD AMOUNT              ; load AMOUNT into the accumulator (for display purposes)
ISZ AMOUNT              ; increment AMOUNT and skip if zero
JMP LOOP                ; loop
HLT
AMOUNT, -1000           ; Number of loops
END                     ; Program is finished

; Commentary:
;    This program is loaded into memory location 2048 (memory location 0 of page 1).
;    During each loop, the link bit is complemented, and the value at memory location
;    AMOUNT is loaded into the accumulator - for display purposes. AMOUNT is incremented
;    and if it becomes 0, the loop will break, and the program ends.
;
;    This could have been made more efficient in several ways but I wanted the lights to
;    flash so I could see that the machine was working. As is, this program runs for
;    about 5000 operations.