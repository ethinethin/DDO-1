START 0
;;; page 0
mem_0, 200                      ; initial value
loops, 0                        ; current loop index
max_loops, 2                    ; max number of loops
p1loc, 2048                     ; store address of page 1
p2loc, 4096                     ; store address of page 2
PPD                             ; pad to page 1
;;; page 1
CLA                             ; add var1 to mem_0
TAD var1                        ; loop 1: mem_0 = 200 + 317 = 517
TAD mem_0                       ; loop 2: mem_0 = 607 + 317 = 924
DCA mem_0
JMP (p2loc)                     ; jump to page 2
var1, 317
PPD                             ; pad to page 2
;;; page 2
CLA                             ; add var2 to mem_0
TAD var2                        ; loop 1: mem_0 = 517 + 90 = 607
TAD mem_0                       ; loop 2: mem_0 = 924 + 90 = 1013
DCA mem_0
ISZ loops                       ; increase loop index
NOP                             ; NOP as a placeholder to prevent skip (not necessary)
TAD max_loops                   ; check if max_loops == loops
CMA
IAC
TAD loops
SZA                             ; they are equal, skip the next instruction
JMP (p1loc)                     ; jump to page 1
CLA CLL                         ; load mem_0 to AC for visualization
TAD mem_0
HLT
var2, 90
END

; This is great.. it works.. I store a variable (mem_0), a loop counter (loops),
; and the max number of loops (maxloops) on page 0. I also store variables with
; the memory locations of page 1 and page 2. On page 1, I add a variable (var1)
; to mem_0, then store in mem_0, then jump to page 2 via the variable stored in
; page 0. On page 2, I add var2 to mem_0, then store it. Then I increase loops
; and store it, then check if it's equal to max loops. If it's not, I jump back
; to page 1.
;
; The final read out should be:
;    AC=0000001111110110 (1014)
;    PC=0001000000001111 (page 2, location 15 (HLT + 1)) 