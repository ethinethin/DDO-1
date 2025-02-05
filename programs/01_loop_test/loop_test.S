;;;;
; loop test - test a basic loop
;;;;
        START   2048            ; Start on page 1
loop,
        CLA CML                 ; complement the link bit
        ISZ     counter         ; increment counter
        NOP                     ; placeholder for skip (not necessary)
        CLA CLL
        TAD     max_loops
        CMA IAC                 ; convert max_loops to negative
        TAD     counter         ; see if counter and max_loops are equal
        SZA                     ; skip if AC = 0
        JMP     loop            ; loop
        CLA CLL
        TAD     counter         ; load counter for display purposes
        HLT
counter,        0               ; loop index
max_loops,      1000            ; Number of loops
        END                     ; Program is finished

; Commentary:
;    This program is loaded into memory location 2048 (memory location 0 of page
;    1). During each loop, the link bit is complemented, and the value at memory
;    location counter is increased. max_loops is converted to a negative number
;    each loop and added to counter. If the result is 0, the loop will break and
;    the program ends.
;
;    The main purpose of this program is to make the L-bit indicator to
;    repeatedly flash on and off.
