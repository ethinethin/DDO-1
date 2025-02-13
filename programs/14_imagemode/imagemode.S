;;;;
; imagemode - demonstrate image mode - draw a pixel of most colors on each line
;;;;
        START   2048
        CLA IAC                 ; load the constant 1
image_mode_l,
        VSF                     ; wait for device to be ready
        JMP     image_mode_l
        VMS                     ; set video mode to image mode
loop,
        JMS     set_color       ; set the current render color
        JMS     draw_pixel      ; draw a pixel
        JMS     check_line      ; check if done line and continue
        JMS     check_done      ; check if done page
        JMP     loop
set_color,      0
        CLA
        TAD     color
set_color_l,
        VSF                     ; wait for device to be ready
        JMP     set_color_l
        VRC                     ; set render color
        ISZ     color           ; increase current color and ignore skip
        NOP
        JMP     (set_color)
draw_pixel,     0
        CLA IAC                 ; load the constant 1
draw_pixel_l,
        VSF                     ; wait for device to be ready
        JMP     draw_pixel_l
        VDC                     ; draw at cursor location
        JMP     (draw_pixel)
check_line,     0
        CLA                     ; first check if color == max_color
        TAD     max_color
        CMA IAC
        TAD     color
        SZA                     ; if it is, skip the return
        JMP     (check_line)
        DCA     color           ; set color = 0
        ISZ     cur_y           ; increase current y and ignore skip
        NOP
        TAD     cur_y           ; load current y and byteswap
        BSW
check_line_l,
        VSF                     ; wait for device to be ready
        JMP     check_line_l
        VMC                     ; move cursor to (0, cur_y)
        JMP     (check_line)
check_done,     0
        CLA                     ; first, check if cur_y == max_y
        TAD     max_y
        CMA IAC
        TAD     cur_y
        SNA                     ; if not equal, skip next line
        JMP     finished        ; if equal, jump to finished, we're done
        JMP     (check_done)
finished,
        HLT
color,          0               ; starting color
max_color,      239             ; maximum color
cur_y,          0               ; current row
max_y,          159
        END
