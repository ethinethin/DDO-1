;;;;
; textcolors - output all text colors
;;;;

        START   2048
        CLA
loop,
        JMS     change_color    ; change color
        JMS     output_m        ; output a characyer
        ISZ     index           ; increase index and ignore skip
        NOP
        CLA
        TAD     max_loops       ; check index vs. max_loops
        CMA IAC
        TAD     index
        SZA                     ; skip if index = max_loops
        JMP     loop
        DCA     index           ; reset index
        DCA     color           ; reset color
clearscreen_l,
        VSF                     ; ensure device is ready
        JMP     clearscreen_l
        VMS                     ; set text mode (clears screen)
resetcursor_l
        VSF                     ; ensure device is ready
        JMP     resetcursor_l
        VMC                     ; move cursor to (0, 0)
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
index,          0
max_loops,      1999
        END
