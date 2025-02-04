# Assembler

See MANUAL.md for operation instructions

## Problem:

Label names can get confused if similar, e.g. "loop2" may be read as "loop"; I need to correct the string logic for this, but for now, I will just keep labels different ("loop1" and "loop2" would fix this problem)

## This is not yet implemented:

Store addresses (via labels) after labels, e.g.:

    ; on page 0
    page2_label_location, page2_label
    ; ...
    ; on page 2
    page2_label,

which would allow jumping to page2_label from anywhere, via `JMP (page2_label_location)`.

All keywords should be capitalized and all labels should be lowercase (not mandatory but helpful). Indirection is specified by brackets, e.g.:

    JMP label              ; jump to address of label
    JMP (label)            ; jump to address stored in label

See examples in ../programs/ for formatting recommendations/standards.