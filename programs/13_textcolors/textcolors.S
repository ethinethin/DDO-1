;;;;
; textcolors - output all text colors
;;;;

        START   2048
        CLA
loop,
        JMS     change_color
        JMS     output_m
        JMP     loop
change_color,   0
        CLA
        TAD     color           ; load current color
change_color_l,
        VSF                     ; ensure device is ready
        JMP     change_color_l
        VRC                     ; set render color
        ISZ     color           ; increment current color
        NOP                     ; ignore skip
        JMP     (change_color)
output_m,       0
        CLA
        TAD     out_char
output_m_l,
        VSF                     ; ensure device is ready
        JMP     output_m_l
        VDC                     ; draw character at cursor
        JMP     (output_m)
out_char,       48              ; output a '0' (ASCII 48)
color,          0
max_color,      256
        END
