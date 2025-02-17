; rng - a pseudo-random number generator using a table with random values. The
;         value is generated based on timing of user input: each tick while
;         waiting for input increments the position in the rng table. The rand
;         function divides the table value by 10

        START   2048
        ; load address of rng table and store in auto-increment register 17
        CLA
        TAD     rng_table
        DCA     17     
; main loop - cycle through all the actions
main_loop,
        ; load and draw string prompt
        CLA
        TAD     string_prompt
        DCA     16
        JMS     outp_str
        ; poll for input
        CLA
        JMS     user_input
        ; calculate random value
        CLA
        JMS     rand
        ; output random value
        CLA
        TAD     string_rand
        DCA     16
        JMS     outp_str
        CLA
        TAD     rand_val
        TAD     char_ASCII
        JMS     outp_char
        CLA
        TAD     char_newline
        JMS     outp_char
        JMP     main_loop

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

; user_input - poll for user input and store in buffer
user_input,     0
        CLA
        TAD     buffer          ; load buffer location
        DCA     16              ;   and store in 16
key_enter,
key_enter_l,
        JMS     rng_increment   ; increment rng table
        KSF                     ; wait until tty kbd device is accepting input
        JMP     key_enter_l
        KRB                     ; load character into accumulator
        DCA     tmp             ; store it in tmp
        ; check if it was a newline
        TAD     char_newline
        CMA IAC
        TAD     tmp
        SNA                     ; if non-zero, was not a newline
        JMP     input_done      ; if zero, it was a newline
        ; check if it was a backspace
        CLA
        TAD     char_backspace
        CMA IAC
        TAD     tmp
        SNA                     ; if non-zero, was not a backspace
        JMP     backspace       ; if zero, it was a backspace
        ; it was not newline/backspace - output it and save to buffer
        CLA
        TAD     tmp             ; load the character entered
        JMS     outp_char       ;   output it to the screen
        DCA     (16)            ;   store in the buffer
        JMP     key_enter       ;   and loop for next character
input_done,
        DCA     (16)            ; store string terminator in buffer
        TAD     char_newline    ; load newline
        JMS     outp_char       ;   and output to the screen
        JMP     (user_input)    ;   then return
backspace,
        ; subtract 1 from the current buffer address
        CLA CMA                 ; load constant -1
        TAD     16              ; add address at 16
        DCA     16              ; store address at 16
        ; output a backspace character to the screen
        TAD     char_backspace
        JMS     outp_char
        CLA
        ; re-enter loop
        JMP     key_enter
tmp,            0

; rng_increment - increment the current position in the rng table
rng_increment,  0
        CLA
        TAD     (17)
        DCA     rng_val         ; store current value
        ISZ     rng_index       ; increment index and ignore skips
        NOP
        ; check if index is at max
        TAD     rng_max
        CMA IAC
        TAD     rng_index
        ; if zero, they are the same, so need to reset
        SNA
        JMP     reset_table
        CLA
        JMP     (rng_increment)
reset_table,
        ; reset rng table
        DCA     rng_index
        TAD     rng_table
        DCA     17
        JMP     (rng_increment)

; rand - generate a random number between 0 and 9 and store in rand_val
rand,           0
        CLA
        ; start with 0 rand_val
        DCA     rand_val
        ; load current rng table value
        TAD     rng_val
rand_l,
        ; subtract 10 from table value (basically dividing by 10 with this loop)
        TAD     sub_10
        ; if < 0, we're done
        SPA
        JMP     (rand)
        ; not done, increase rand_val
        ISZ     rand_val
        NOP
        JMP     rand_l
sub_10,         -10

; random values table and index variables
rng_table,      .
        43 45 17 26
        65 55 44 29
        92 34 33 11
        60 94 49 85
        51 25 92 14
        4 8 5 36
        44 12 64 18
        50 38 73 29
        7 18 68 25
        60 76 60 81
        3 42 82 27
        95 23 15 61
        57 65 89 60
        83 87 5 96
        13 38 31 16
        92 25 51 43
        70 16 82 36
        51 5 52 22
        45 99 94 22
        18 52 90 25
        54 78 27 66
        26 4 79 63
        99 44 0 58
        37 11 24 83
        71 51 47 83
        74 44 31 94
        47 18 12 66
        48 88 20 25
        94 97 69 82
        86 64 55 62
        60 88 67 8
        53 66 20 51
        67 32 23 23
        55 59 50 17
        25 34 18 54
        27 97 56 34
        76 57 40 34
        79 72 95 27
        75 4 52 32
        79 20 0 15
        91 12 95 66
        13 91 89 45
        56 57 70 99
        8 32 94 57
        54 29 10 0
        52 38 23 83
        35 27 48 94
        48 30 80 78
        53 67 82 66
        68 64 23 3
rng_index,      0
rng_max,        200
rng_val,        0
; variable to store calculated random value
rand_val,       0
; string and character literals
string_prompt,  .               ; ASCII characters for " ] "
        32 93 32 0
string_rand,    .               ; ASCII character for " The random value is: "
        32 84 104 101
        32 114 97 110
        100 111 109 32
        118 97 108 117
        101 32 105 115
        58 32 0
char_ASCII,     48              ; ASCII value to add to numbers to display
char_newline,   10              ; ASCII value for \n character
char_backspace, 8               ; ASCII character for '\b'
; buffer for user input
buffer,         .               ; Buffer to hold user input
        PAD     80
        END
