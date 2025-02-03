# Assembler

Run by ./asm file.S

Output will be a.bin ... maybe this should be a.out for historical reasons?

Problems/To be implemented:
- More than 2 words (e.g. combined microcodes) not implemented yet
- Store addresses (via labels) after labels, e.g.

    ; on page 0
    page2_label_location, page2_label
    ; ...
    ; on page 2
    page2_label,

(which would allow jumping to page2_label from anywhere, via `JMP (page2_label_location)`)

All keywords should be capitalized and all labels would be best if lowercase. Indirection is specified by brackets, e.g.:

JMP label              ; jump to address of label
JMP (label)            ; jump to address stored in label

Does this work? Well, the tests I've written so far work. They are in ../programs, and are fairly simple, but they seem to work.

A problem, similar loop labels may give grief, e.g. "loop" vs. "loop2", any instances of "loop2" will be read as "loop" .. I need to fix the string logic, should be easy