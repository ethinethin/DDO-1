; itoa - function to convert unsigned integers to ASCII strings

        START   2048
        CLA
        ; set buffer address in auto-inc register 16
        TAD     buffer
        DCA     16
        ; load var1 and convert to string with uiota
        TAD     var1
        DCA     uiota_n
        JMS     uiota
        ; print the result
        CLA
        TAD     buffer
        DCA     16
        JMS     outp_str
        HLT
; test value
var1,           40641
; buffer large enough to hold 6 characters and a string terminator
buffer,         .
        0 0 0 0
        0 0 0

; outp_str - output the string pointed to by the auto increment register at 16
outp_str,       0
outp_loop,
        CLA
        TAD     (16)
        SNA                     ; if current character is 0, return
        JMP     (outp_str)
        JMS     outp_char       ; otherwise, output current character and loop
        JMP     outp_loop

; outp_char - output the character in the accumulator
outp_char,      0
outp_char_l,
        VSF                     ; wait for device to be ready
        JMP     outp_char_l
        VDC                     ; draw at cursor
        JMP     (outp_char)

; uiota - convert unsigned integer in uiota_n to ASCII string and store at (16)
uiota,          .
; 10,000's place
uiota_10k_rdy,
        ; clear tmp variable and leading 0 flag
        CLA CLL
        DCA     uiota_tmp
        DCA     uiota_leading
        ; load value and go into 10k loop
        TAD     uiota_n
uiota_10k_l,
        ; clear link bit and update the value
        CLL
        JMS     update_uiota_n
        TAD     const_n10000
        ; if L = 0, you have finished this round
        SNL
        JMP     uiota_10k_done
        ; increase counter and loop
        ISZ     uiota_tmp
        NOP
        JMP     uiota_10k_l
uiota_10k_done,
        CLA CLL
        TAD     uiota_tmp
        ; if tmp = 0, just move on to 1k loop, to skip leading zeroes
        SNA
        JMP     uiota_1k_rdy
        ; otherwise, add ASCII adjustment to it and store in (16)
        TAD     char_ASCII
        DCA     (16)
        ; turn on leading 0 flag
        ISZ     uiota_leading
        NOP
; 1,000's place
uiota_1k_rdy,
        CLA CLL
        DCA     uiota_tmp
        TAD     uiota_n
uiota_1k_l,
        ; clear link bit and update the value
        CLL
        JMS     update_uiota_n
        TAD     const_n1000
        ; if L = 0, you have finished this round
        SNL
        JMP     uiota_1k_done
        ; increase counter and loop
        ISZ     uiota_tmp
        NOP
        JMP     uiota_1k_l
uiota_1k_done,
        CLA
        TAD     uiota_tmp
        ; if tmp = 0, check leading 0 flag
        SZA
        JMP     uiota_1k_output
        ; if flag is 0, just move on
        TAD     uiota_leading
        SNA
        JMP     uiota_100_rdy
        ; otherwise we will fall through and add it
        CLA
        TAD     uiota_tmp
uiota_1k_output,
        TAD     char_ASCII
        DCA     (16)
; 100's place
uiota_100_rdy,
        CLA CLL
        DCA     uiota_tmp
        TAD     uiota_n
uiota_100_l,
        ; clear link bit and update the value
        CLL
        JMS     update_uiota_n
        TAD     const_n100
        ; if L = 0, you have finished this round
        SNL
        JMP     uiota_100_done
        ; increase counter and loop
        ISZ     uiota_tmp
        NOP
        JMP     uiota_100_l
uiota_100_done,
        CLA
        TAD     uiota_tmp
        ; if tmp = 0, check leading 0 flag
        SZA
        JMP     uiota_100_output
        ; if flag is 0, just move on
        TAD     uiota_leading
        SNA
        JMP     uiota_10_rdy
        ; otherwise we will fall through and add it
        CLA
        TAD     uiota_tmp
uiota_100_output,
        TAD     char_ASCII
        DCA     (16)
; 10's place
uiota_10_rdy,
        CLA CLL
        DCA     uiota_tmp
        TAD     uiota_n
uiota_10_l,
        ; clear link bit and update the value
        CLL
        JMS     update_uiota_n
        TAD     const_n10
        ; if L = 0, you have finished this round
        SNL
        JMP     uiota_10_done
        ; increase counter and loop
        ISZ     uiota_tmp
        NOP
        JMP     uiota_10_l
uiota_10_done,
        CLA
        TAD     uiota_tmp
        ; if tmp = 0, check leading 0 flag
        SZA
        JMP     uiota_10_output
        ; if flag is 0, just move on
        TAD     uiota_leading
        SNA
        JMP     uiota_1_rdy
        ; otherwise we will fall through and add it
        CLA
        TAD     uiota_tmp
uiota_10_output,
        TAD     char_ASCII
        DCA     (16)
; 1's place - whatever is left can be added to (16)
uiota_1_rdy,
        CLA CLL
        TAD     uiota_n
        TAD     char_ASCII
        DCA     (16)
uiota_done,
        ; append a terminal 0 for a string terminator
        DCA     (16)
        JMP     (uiota)
; helper subroutine to store and reload current value
update_uiota_n, .
        DCA     uiota_n
        TAD     uiota_n
        JMP     (update_uiota_n)
; subroutine variables
uiota_n,        0
uiota_tmp,      0
uiota_leading,  0

; constants
const_n10000,   -10000
const_n1000,    -1000
const_n100,     -100
const_n10,      -10
char_ASCII,     48

        END