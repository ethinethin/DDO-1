;;;;
; textmode - take text entry from the user and output it to the monitor
;;;;

        START 2048
        CLA
loop,
        JMS     get_k           ; wait for input from keyboard
        JMS     output_k        ; output the character entered
        JMP     loop            ; loop forever
get_k,          0
        CLA
get_k_l,
        KSF                     ; wait for keyboard input
        JMP     get_k_l
        KRB                     ; input character into AC
        DCA     cur_char        ; store at cur_char
        JMP     (get_k)         ; return
output_k,       0
        TAD     cyan            ; load color and byte swap
        BSW
        TAD     cur_char        ; add character to print out
        VIC                     ; video insert character
        JMP     (output_k)      ; return
cyan,           9
        END
