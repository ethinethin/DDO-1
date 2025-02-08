# DDO-1 Assembler

See MANUAL.md for operation instructions

# Features to be implemented

## Strings and characters

There's not yet logic to parse strings or single characters, but up to four
naked values can be given per line, e.g.:

    72 101 108 108          ; ASCII: 'H' 'e' 'l' 'l'
    111 44 32 87            ; ASCII: 'o' ',' ' ' 'W'
    111 114 108 100         ; ASCII: 'o' 'r' 'l' 'd'
    33 10 0                 ; ASCII: '!' '\n' NUL

## Operations following labels

Operations cannot be declared on the same line as labels yet, and will instead
be interpreted as a value. For example:

    loop,   NOP             ; Will be interpreted the same as loop, 0

For now, the operation must be on the next line, e.g.:

    loop,
            NOP             ; This is fine

It just makes parsing labels more complicated so I haven't implemented it yet,
but I have already been guilty of accidentally writing an operation on a label
line. It took me a few minutes to debug that one!

## Address math not implemented

Address math is not implemented yet. For example, the following will not work:

    TAD var2+1             ; load the value 1 after var2 - NOT IMPLEMENTED
    JMP .-1                ; jump to current address minus 1 - NOT IMPLEMENTED

# Style guide:

All keywords should be capitalized and all labels should be lowercase (not mandatory but helpful). Indirection is specified by brackets, e.g.:

    JMP label              ; jump to address of label
    JMP (label)            ; jump to address stored in label

See examples in ../programs/ for formatting recommendations/standards.
