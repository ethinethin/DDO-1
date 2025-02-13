;;;;
; textmode - take text entry from the user and output it to the monitor
;;;;

        START 2048
        CLA
        TAD     color           ; set rendering color to default
setup_l,
        VSF                     ; loop until monitor is ready
        JMP     setup_l
        VRC                     ; change rendering color
loop,
        JMS     get_k           ; wait for input from keyboard
        JMS     output_m        ; output the character entered to monitor
        JMP     loop            ; loop forever
get_k,          0
        CLA
get_k_l,
        KSF                     ; wait for keyboard input
        JMP     get_k_l
        KRB                     ; input character into AC
        DCA     cur_char        ; store at cur_char
        JMP     (get_k)         ; return
output_m,       0
        CLA
output_m_l,
        VSF                     ; skip if device ready
        JMP     output_m_l
        TAD     cur_char        ; load character and output
        VDC                     ; video draw character
        JMP     (output_m)      ; return
color,          31              ; starting color = 0b00011111 (cyan in RGB332)
        END
