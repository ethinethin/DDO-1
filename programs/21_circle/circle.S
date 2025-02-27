; circle.S - implement a circular buffer and skip chain
;

; start at page 1
        START   2048
main,
        ; set up keyboard and monitor buffer pointers
        CLA
        TAD     buffer_loc
        DCA     16
        TAD     buffer_loc
        DCA     17
main_l,
        ; keyboard and monitor skip chain loop
        KSF
        SKP
        JMS     ttyk
        VSF
        SKP
        JMS     mon
        JMP     main_l
buffer_loc,     buffer

; ttyk - handle keyboard input
ttyk,           .
        ; load keystroke into buffer
        CLA
        TAD     16              ; load buffer location
        JMS     wrap_b          ;   ensure in buffer
        DCA     16              ;   store buffer location
        KRB                     ; load keystroke
        DCA     (16)            ;   store in buffer
        ; add a string-terminating 0 after it
        TAD     16
        JMS     wrap_b
        DCA     16
        DCA     (16)
        ; move buffer -1 to before terminator
        CMA
        TAD     16
        JMS     wrap_b
        DCA     16
        JMP     (ttyk)

; mon - handle monitor output
mon,            .
        ; load current character
        CLA
        TAD     17
        JMS     wrap_b
        DCA     17
        TAD     (17)
        ; if it's 0, move the buffer back and leave
        SZA
        JMP     mon_print
        CMA
        TAD     17
        JMS     wrap_b
        DCA     17
        JMP     (mon)
mon_print,
        ; print out to monitor
        VDC
        JMP     (mon)

; wrap_b - wrap address to fit within the circular buffer
wrap_b,         .
        ; mask the address to ensure it's within the buffer size
        AND     (buffer_size_1)
        ; add the page address to ensure it's in the correct page
        TAD     (buffer_page_1)
        JMP     (wrap_b)

; store page 2 variable addresses on page 1
buffer_size_1,  buffer_size
buffer_page_1,  buffer_page

; pad to page 2
        PPD
; buffer location - pad with 8 zeroes
buffer,
        PAD     9
; buffer attributes
buffer_size,    7
buffer_page,    4096
        END

;;
;; This is semi-broken... because of the way auto-increment registers work.
;;   There are 9 zeroes in a row. The first (buffer, addr=4096) never gets
;;   written to. Then the next 8 values do get written to. It's because the 
;;   register increments after writing to it. I feel stupid for not knowing how
;;   to solve this. But at least, for now, this is functioning as a circular
;;   buffer.
;;