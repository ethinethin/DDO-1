;;;;
; page_trav - Demonstrates page traversal by storing addresses that allow
;             movement between pages. It also showcases PPD (Page Pad).
;
; Normally, the assembler prevents access to addresses outside page 0 or the 
; current page. However, storing addresses as data (e.g., storing the address of 
; a label on page 2 in a location on page 1) is allowed.
;;;;

        START   0
;;;;
; page 0 - this space is normally reserved for interrupt handlers and auto-
;          increment registers, but here it is used to store some variables,
;          including addresses
;;;;
mem_0,          200             ; initial value
loops,          0               ; current loop index
max_loops,      2               ; max number of loops
        PPD                     ; pad to page 1
;;;;
; page 1 -- standard program execution entry point
;;;;
page1,
        CLA CLL                 ; add var1 to mem_0
        TAD     var1            ; loop 1: mem_0 = 200 + 317 = 517
        TAD     mem_0           ; loop 2: mem_0 = 607 + 317 = 924
        DCA     mem_0
        JMP     (p2loc)         ; jump to page 2
var1,           317
p2loc,          page2           ; store address of "page2" label
        PPD                     ; pad to page 2
;;;;
; page 2
;;;;
page2,
        CLA                     ; add var2 to mem_0
        TAD     var2            ; loop 1: mem_0 = 517 + 90 = 607
        TAD     mem_0           ; loop 2: mem_0 = 924 + 90 = 1014
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
p1loc,          page1           ; store address of "page1" label
        END

;;;;
; Expected output
;;;;
; The final read out should be:
;    AC=0000001111110110 (1014)
;    PC=0001000000001111 (page 2, location 15 (HLT + 1)) 
