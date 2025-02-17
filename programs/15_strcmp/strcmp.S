; strcmp - example of how to dynamically compare strings.
;
; This program behaves like a terminal prompt. If the user enters "hello" or
;   "world", it results in a match. Any other input is unknown. It currently
;   correctly handles newlines and backspaces.
;
; It does not currently check if more characters are entered than are reserved
;   for the buffer. The buffer is at the end of the program, meaning the user
;   would have to enter tens of thousands of characters for a buffer overflow.
;   But it would be nice to implement buffer checking, then have the buffer
;   region directly after the user_input subroutine.
;
; The stcmp subroutine itself uses two labels, s1 and s2, as arguments. The user
;   stores the addresses of the two strings at those labels, then calls the
;   subroutine. It returns AC = 0 for a match, and AC = 1 for a non-match.

        START   2048
main_loop,
        CLA
        TAD     string_prompt   ; load prompt string address
        DCA     16              ;   and store in 16
        JMS     outp_str        ;   and output to screen
        JMS     user_input      ; poll for user input (stored to buffer)
        ; compare user input to string_hello
        CLA
        TAD     buffer          ; load s1 address
        DCA     s1              ;   and store at s1
        TAD     string_hello    ; load s2 address
        DCA     s2              ;   and store at s2
        JMS     strcmp          ; compare s1 and s2
        SNA                     ; if non-zero, there was no match
        JMP     match           ; if zero, there was a match
        ; compare user input to string_world
        CLA
        TAD     buffer          ; load s1 address
        DCA     s1              ;   and store at s1
        TAD     string_world    ; load s2 address
        DCA     s2              ;   and store at s2
        JMS     strcmp          ; compare s1 and s2
        SNA                     ; if non-zero, there was no match
        JMP     match           ; if zero, there was a match
        ; there was no match, display as such
        CLA
        TAD     string_unknown  ; load unknown string address
        DCA     16              ;   and store in 16
        JMS     outp_str        ; output the string
        CLA
        TAD     char_newline    ; load newline
        JMS     outp_char       ;   and output it
        JMP     main_loop
match,
        CLA
        TAD     string_match    ; load string that we had a match and display
        DCA     16
        JMS     outp_str
        CLA
        TAD     char_newline    ; load a newline and display
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
        KSF                     ; wait until device is accepting input
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

; strcmp - compare strings at s1 and s2, return values: 0 (strings identical),
;            or 1 (strings not identical)
strcmp,         0
        CLA
        TAD     s1              ; load address of string s1
        DCA     16              ;   and save at auto-increment register 16
        TAD     s2              ; load address of string s2
        DCA     17              ;   and save at auto-increment register 17
strcmp_l,
        CLA
        TAD     (16)
        SNA                     ; if zero, end of string
        JMP     end_string
        CMA IAC                 ; make current value negative
        TAD     (17)            ; load character
        SNA                     ; if non-zero, strings not identical
        JMP     strcmp_l        ; characters were the same, check next character
        CLA IAC                 ; load constant 1
        JMP     (strcmp)        ; they were different, return with AC = 1
end_string,
        TAD     (17)            ; load character at 17
        SNA                     ; if non-zero, strings not identical
        JMP     (strcmp)        ; they were identical, return with AC = 0
        CLA IAC                 ; load constant 1
        JMP     (strcmp)        ; they were different, return with AC = 1
s1,     0
s2,     0

; string and character literals
string_prompt,  .               ; ASCII characters for "] "
        93 32 0
string_hello,   .
        104 101 108 108         ; ASCII characters for "hello"
        111 0
string_world,   .               ; ASCII characters for "world"
        119 111 114 108
        100 0
string_match,   .               ; ASCII characters for "* Match"
        42 32 77 97
        116 99 104 0
string_unknown, .               ; ASCII characters for "* Unknown string"
        42 32 85 110
        107 110 111 119
        110 32 115 116
        114 105 110 103
        0
char_newline,   10              ; ASCII character for '\n'
char_backspace, 8               ; ASCII character for '\b'
; buffer for user input
buffer,         .               ; Buffer to hold user input
        PAD     80
        END
