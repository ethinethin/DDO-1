;;;;
; kbd_buffer - fill up a kbd buffer until the user presses enter, then output
;              the contents of the buffer
;;;;

        START 2048
        CLA
        TAD     buffer          ; store address of buffer at auto-increment
        DCA     16              ;   register at memory location 16
loop,
        JMS     get_k           ; wait for input from keyboard
        JMS     output_k        ; output the character entered
        JMS     was_newline     ; check if the character was a newline
        JMP     loop            ; it was not, so loop
get_k,          0
        CLA
get_k_l,
        KSF                     ; wait for keyboard input
        JMP     get_k_l
        KRB                     ; input character into AC
        DCA     cur_char        ; store at cur_char
        TAD     cur_char        ; also store in buffer
        DCA     (16)
        JMP     (get_k)
output_k,       0
        TAD     cur_char        ; load character to print out
output_k_l,
        TSF                     ; wait until printer is ready
        JMP     output_k_l
        TLS                     ; print out character
        JMP     (output_k)
was_newline,    0
        CMA IAC                 ; make negative and add newline
        TAD     newline
        SZA                     ; if the sum is zero, newline was entered
        JMP     (was_newline)   ; it was not zero, return to loop
        DCA     (16)            ; put in a string terminator at the end of the
        JMP     print_buf       ;   buffer
print_buf,
        TAD     buffer          ; store the address of buffer in the auto-
        DCA     16              ;   increment register at memory address 16
print_buf_loop,
        CLA
        TAD     (16)
        SNA                     ; if non-zero, skip
        JMP     done            ; we hit a 0, we are done
        DCA     cur_char        ; store character and print with the output_k
        JMS     output_k        ;   subroutine
        JMP     print_buf_loop  ; loop until done
done,
        TAD     buffer          ; store address of buffer at auto-increment
        DCA     16              ;   register at memory location 16
        JMP     loop            ; re-enter the main loop
cur_char,       0
newline,        10
buffer,         .
        PAD     80              ; make a buffer with 80 zeroes
        END
