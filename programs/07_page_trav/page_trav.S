;;;;
; page_trav - demonstrate page traversal by storing important addresses
;             on page 0 to allow movement among pages; the convenient
;             PPD (page pad) command is also demonstrated here
;;;;
        START   0
;;;;
; page 0 - this space is normally reserved for interrupt handlers and
;          auto-increment registers, but we are using it to store some
;          variables, including addresses
;;;;
mem_0,          200             ; initial value
loops,          0               ; current loop index
max_loops,      2               ; max number of loops
p1loc,          2048            ; store address of page 1
p2loc,          4096            ; store address of page 2
        PPD                     ; pad to page 1
;;;;
; page 1 -- standard program execution entry point
;;;;
        CLA CLL                 ; add var1 to mem_0
        TAD     var1            ; loop 1: mem_0 = 200 + 317 = 517
        TAD     mem_0           ; loop 2: mem_0 = 607 + 317 = 924
        DCA     mem_0
        JMP     (p2loc)         ; jump to page 2
var1,           317
        PPD                     ; pad to page 2
;;;;
; page 2
;;;;
        CLA                     ; add var2 to mem_0
        TAD     var2            ; loop 1: mem_0 = 517 + 90 = 607
        TAD     mem_0           ; loop 2: mem_0 = 924 + 90 = 1013
        DCA     mem_0
        ISZ     loops           ; increase loop index
        NOP                     ; prevent skip (not necessary)
        TAD     max_loops       ; check if max_loops == loops
        CMA
        IAC
        TAD     loops
        SZA                     ; they are equal, skip the next instruction
        JMP     (p1loc)         ; jump to page 1
        CLA CLL                 ; load mem_0 to AC for visualization
        TAD     mem_0
        HLT
var2,           90
        END

;;;;
; Expected output
;;;;
; The final read out should be:
;    AC=0000001111110110 (1014)
;    PC=0001000000001111 (page 2, location 15 (HLT + 1)) 