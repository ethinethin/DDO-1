;;;;
; kbd - testing the input from the teletype keyboard
;;;;

        START 2048
loop,
        CLA
        JMS     get_k
        JMS     output_k
        JMP     loop
get_k,          0
get_k_l,
        KSF                     ; poll until keyboard input
        JMP     get_k_l
        KRB                     ; get input
        DCA     stored_k        ; store the keyboard
        JMP     (get_k)
stored_k,       0
output_k,       0
output_k_l,
        TSF                     ; wait until printer is ready
        JMP     output_k_l
        TAD     stored_k        ; print stored character
        TLS
        JMP     (output_k)
