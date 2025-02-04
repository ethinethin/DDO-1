;;;;
; conditionals - convoluted code to test some conditionals
;;;;
        START   2048
loop,
        CLA CLL                 ; add addval to var1 and store in var1
        TAD     var1
        TAD     addval
        DCA     var1
        SNL                     ; skip next instruction if L = 1 (AC overflow)
        JMP     loop            ; jump to loop if L = 0
        CLA
        TAD     addval          ; double addval and re-store it
        TAD     addval
        DCA     addval
        JMS     check_if_done   ; see if we have reached final condition
        JMP     loop            ; loop again if we returned
check_if_done,  0
        TAD     maxval          ; load maxval, ones complement, and increment
        CMA IAC
        TAD     addval          ; check if addval = maxval
        SZA                     ; skip if AC = 0 (addval = maxval)
        JMP     (check_if_done) ; if not, return 
        TAD     addval          ; exit loop and reload addval to display
        HLT                     ; halt operation
var1,           0
addval,         1
maxval,         1024
        END