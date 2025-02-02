# Assembler

Run by ./asm file.S

Output will be a.bin

Problems/To be implemented:
- Page 0 vs. current page not implemented yet (just assuming page 1 for everything)
- More than 2 words (e.g. combined microcodes) not implemented yet

All keywords should be capitalized and all labels would be best if lowercase. Indirection is specified by brackets, e.g.:

JMP label              ; jump to address of label
JMP (label)            ; jump to address stored in label

Does this work? Well, the tests I've written so far work. They are in ../programs, and are fairly simple, but they seem to work.