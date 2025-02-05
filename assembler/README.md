# DDO-1 Assembler

See MANUAL.md for operation instructions

# Features to be implemented

## Values alone on lines not implemented

I want to implement values by themselves on lines, probably up to 4, e.g.:

    0 1 2 4                ; four words, specifying the values 0, 1, 2, and 4, respectively

I will probably need to add logic for storing ASCII strings "like this" and characters like this: 'c'

## Address math not implemented

Address math is not implemented yet. For example, the following will not work:

    TAD var2+1             ; load the value 1 after var2 - NOT IMPLEMENTED
    JMP .-1                ; jump to current address minus 1 - NOT IMPLEMENTED

# Style guide:

All keywords should be capitalized and all labels should be lowercase (not mandatory but helpful). Indirection is specified by brackets, e.g.:

    JMP label              ; jump to address of label
    JMP (label)            ; jump to address stored in label

See examples in ../programs/ for formatting recommendations/standards.
