; rng2 - a pseudo-random number generator using a table with random values. The
;         value is generated based on timing of user input: each tick while
;         waiting for input increments the position in the rng table. 
;
; This is a bit ugly, and it could probably be cleaned up a bit. But I think it
;   works, and I have tested out some edge cases. I added more random values to
;   the rng table and have added all values equally: (0-99) x 4, in a random
;   order. So it is maybe a little more random than the last program.
;
; This ends up being 2702 words long. I need to have a better system for
;   tracking pages in the assembler, because I wasn't sure if I was still on
;   page 1 or not. Page 1 goes from word 2048-4095, so this was safely in a
;   single page, but this is still over 600 words, so probably the longest one
;   I've written yet (thanks mainly to the large random vals table).

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
        ; convert the integer to ASCII
        CLA
        TAD     rng_val
        DCA     itoa_n
        JMS     itoa
        ; output random value string
        CLA
        TAD     string_rand
        DCA     16
        JMS     outp_str
        ; output result of itoa
        CLA
        TAD     itoa_result
        DCA     16
        JMS     outp_str
        ; output a newline
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

; itoa - convert integer in itoa_n to an ASCII string; this only needs to go up
;          to two characters because the values will be 0-99
itoa,           .
        ; store result address in auto increment register 16 and clear tmp
        CLA
        TAD     itoa_result
        DCA     16
itoa_10,
        ; clear tmp then load number to work on it
        DCA     itoa_tmp
        TAD     itoa_n
itoa_10_l,
        ; subtract 10 at a time to calculate 10s place
        DCA     itoa_n          ; at the beginning of each loop, store the value
        TAD     itoa_n          ;   and reload it
        TAD     itoa_minus_10   ;   then subtract 10
        ; if negative, move on to 1s place
        SPA
        JMP     itoa_10_done
        ISZ     itoa_tmp
        NOP
        JMP     itoa_10_l
itoa_10_done,
        CLA
        TAD     itoa_tmp
        ; if zero, we don't need to store it, to avoid the leading zero
        SNA
        JMP     itoa_1
        ; otherwise, add ASCII adjustment and store it in itoa_result
        TAD     char_ASCII
        DCA     (16)
itoa_1,
        ; clear temp then load value to work on
        DCA     itoa_tmp
        TAD     itoa_n
        ; if it's zero just skip this part
        SNA
        JMP     itoa_1_done
itoa_1_l,
        ; add 1 to the tmp
        ISZ     itoa_tmp
        NOP
        ; subtract 1 at a time to calculate 1s place
        DCA     itoa_n          ; re-store at the beginning of each loop and
        TAD     itoa_n          ;   reload it
        TAD     itoa_minus_1
        ; if zero, we are done, else loop
        SNA
        JMP     itoa_1_done
        JMP     itoa_1_l
itoa_1_done,
        ; load tmp, add ASCII adjustment, then store it in itoa_result
        CLA
        TAD     itoa_tmp
        TAD     char_ASCII
        DCA     (16)
        ; store a string terminating 0
        DCA     (16)
        JMP     (itoa)
itoa_n,         0
itoa_result,    .
        0 0 0
itoa_minus_10,  -10
itoa_minus_1,   -1
itoa_tmp,       0


; random values table and index variables
rng_table,      .
        48 20 52 73
        27 45 21 99
        27 2 23 26
        55 88 64 9
        19 69 66 96
        86 82 8 4
        11 47 59 58
        13 97 89 57
        98 71 35 91
        23 94 77 74
        96 3 25 54
        14 44 13 61
        29 2 57 85
        37 39 76 32
        2 95 61 28
        56 75 64 34
        20 15 19 47
        24 17 51 81
        31 84 95 48
        56 99 70 90
        68 80 53 87
        7 45 59 84
        7 25 11 71
        12 0 40 5
        67 41 90 32
        50 39 24 90
        31 37 13 10
        52 15 14 41
        86 23 64 13
        94 5 51 70
        33 87 53 67
        91 40 92 65
        51 97 21 6
        93 61 23 93
        83 25 84 30
        36 60 36 77
        76 55 93 96
        28 26 81 17
        26 30 36 42
        79 16 80 86
        26 29 43 22
        75 74 8 0
        44 61 12 22
        10 65 62 92
        60 15 6 31
        60 27 1 58
        65 0 90 50
        29 11 73 68
        50 78 63 85
        46 55 97 43
        9 98 97 12
        40 18 18 92
        63 83 60 42
        1 70 49 53
        49 17 54 82
        57 72 4 56
        84 66 1 6
        41 24 38 54
        17 83 35 47
        70 12 39 93
        32 85 77 53
        69 37 5 87
        50 89 14 4
        10 46 65 58
        62 38 11 3
        27 87 3 34
        10 7 99 20
        79 59 74 45
        46 98 79 89
        19 52 82 54
        81 75 83 34
        52 56 91 44
        9 62 44 95
        78 21 43 63
        6 45 75 62
        95 38 35 42
        94 48 37 73
        32 80 8 41
        33 63 24 69
        96 72 18 78
        28 9 19 88
        89 1 16 42
        73 30 88 49
        36 47 79 94
        58 18 25 76
        98 72 82 51
        48 67 85 16
        78 31 57 99
        16 81 68 4
        59 71 33 91
        71 43 55 21
        68 15 2 86
        66 64 39 88
        29 20 74 5
        92 38 69 40
        22 76 66 67
        28 14 72 77
        35 7 33 46
        22 30 0 80
        34 49 8 3
rng_index,      0
rng_max,        400
rng_val,        0
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
