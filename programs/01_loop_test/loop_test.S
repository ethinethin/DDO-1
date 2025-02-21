; loop_test - test a basic loop that loops 1000 times
;
; During each loop, the link bit is complemented and the counter is increased.
;   When the counter reaches 1000 (max_loops), the program ends. The program
;   entry point is 2048 (memory 0 of page 1). The check against max_loops is
;   done by calculating the two's complement negative of max_loops, by one's
;   complementing it and incrementing it one (CMA IAC).
;
; The purpose of this program is to make the L-bit indicator on the panel
;   repeatedly flash on and off, although it may not be visible if the operating
;   speed of the simulator is set too high.
;

        START   2048            ; Start on page 1
loop,
        CLA CML
        ISZ     counter         ; increment counter
        NOP                     ; ignore ISZ skip (not necessary)
        ; load max_loops, make negative, then add to counter
        CLA CLL
        TAD     max_loops
        CMA IAC
        TAD     counter
        ; if AC != 0, loop; otherwise load counter and finish
        SZA
        JMP     loop
        CLA CLL
        TAD     counter
        HLT
counter,        0               ; loop index
max_loops,      1000            ; number of loops
        END
