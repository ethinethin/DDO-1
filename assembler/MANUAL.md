# Assembler Manual

This is the manual for the DDO-1 assembler. This program and manual are works in progress, and are licensed under the same terms as the DDO-1 project to which they belong.

## Programming in this assembly language

### Style and whitespace

For readability, each line should be broken up into 5 columns, each starting at a different 8-character tab-stop. The columns are:

1. Labels, start at character 0
2. Operations, start at character 8
3. Values/Operands, start at character 16
4. Comments, start at character 32

The fourth column (starting at character 24) is left blank to leave room for longer operations/values.

Whitespace, including both tabs and spaces, is ignored while assembling programs, so these are just style recommendations, not required.

### Comments

Comments can occur anywhere and are denoted by the character `;`. Any characters following `;` on a line are ignored.

### START and END

`START` is used at the very beginning of every assembly file and END is used at the end. For `START`, specify the memory location that assembly should begin, e.g.:

            START   0               ; Start at memory location 0 (memory location 0 of page 0)
            START   2048            ; Start at memory location 2048 (memory location 0 of page 1)

END is used to signify that assembly is finished. You can include documentation or commentary below this keyword, because nothing after it will be assembled.

### Padding with 0s

`PAD` and `PPD` (page pad) are used to pad sections of memory with zeroes. `PAD` is used with a value to specify how many words of memory to pad with zeroes, while `PPD` is a stand-alone operation that pads the current page with zeroes, ensuring the next word starts on the first word of the next page. For example:

            START   0               ; Start at memory location 0 (memory location 0 of page 0)
            PAD     100             ; pad with 100 zeroes (memory locations 0-99)
            CLA                     ; this instruction is in memory location 100
            PPD                     ; pad with zeroes until the next page
            CLA                     ; this instruction is in memory location 2048

### Labels

Labels are specified with a comma. Lowercase is preferred for labels, and they can be declared in two ways: (1) with a value (to specify the value be stored at the address of the label), (2) with another label to specify the address of that label, (3) with a . to indicate the self-address of the label, or (4) on a line by itself, indicating the label's address me the memory location of the next instruction. Examples:

    var1,           0               ; Store the value 0, var1 refers to this memory location
    var2,           var1            ; Store the address of var1 in var2
    var3,           .               ; Store the address of var3 in var3
    loop,                           ; A label likely used as a loop point, refers to the address of the next word

At assembly time, labels are converted into memory addresses, with label names lost in the assembled binary.

**Important note: labels cannot be combined with operations.**

    bad,    CLA                     ; this is invalid

It will be interpreted as the numeric value of the operation, so the above will be interpreted as:

    bad,            0               ; the value 0 stored at the address specified by the label bad

### Raw values

Raw values can be provided either one with a label or up to four per line. Examples:

    var1,           0               ; Store value 0, var1 refers to this memory location
            10 20 30 40             ; The values 10, 20, 30, 40 stored sequentially at this location
            50 60                   ; The values 50, 60 stored sequentially here

This works well for storing strings with a self-addressed label (to be used via auto-increment registers), e.g.:

    hello,          .               ; hello contains the address of itself
            72 101 108 108          ; ASCII: 'H' 'e' 'l' 'l'
            111 44 32 87            ; ASCII: 'o' ',' ' ' 'W'
            111 114 108 100         ; ASCII: 'o' 'r' 'l' 'd'
            33 10 0                 ; ASCII: '!' '\n' NUL

Stylistically, raw values after a label should be in column 3 (the "Values/Operands" column) while raw values by themselves should be in column 2 (the "operations" column, or starting at character 8 of the line). This is mainly to allow room between the values and the comments column.

### Op codes and indirection

The memory reference instruction (MRI) opcodes are given an operand: either a numeric memory address or a label.

|Mnemonic|Operation|
|--------|---------|
|AND|logical and operand and AC|
|TAD|two's complement add operand to <L, AC>|
|ISZ|increment value at address and skip next instruction if result is 0|
|DCA|store AC in address, clear AC|
|JMP|jump to address|
|JMS|jump to subroutine (return address stored at word 0 of subroutine, execution begins at word 1)|

The instruction acts on the value stored in memory at that address. If parenthesis are used around the address/label, then the instruction acts on the value stored in memory at the address stored in that memory location instead (indirection). Examples:

            TAD     1000            ; add the value stored at address 1000 to the AC
            TAD     var1            ; add the value stored at the label var1 to the AC
            TAD     (1000)          ; add the value stored at the address in memory at address 1000
            TAD     (var1)          ; add the value stored at the address in memory at label var1

Memory locations in assembly are absolute, not relative. Therefore:

            JMP     0               ; jump to page 0, memory location 0 (valid on all pages)
            JMP     2048            ; jump to page 1, memory location 0 (only valid on page 1)

If an instruction on the last word of a page is executed, execution will continue onto the next page. Subroutines should not span two pages, or the return address will be inaccessible without purposefully used indirection (i.e. storing word 0 of the subroutine in a location on the next page). To avoid this issue, and if memory allows, it is a good practice to pad to the end of the current page and start the subroutine on a new page.

    ; somewhere late on page 1
            CLA CLL
            TAD     var1            ; load value into AC
            JMS     (nega_loc)      ; indirectly call the make_nega subroutine
            HLT
    var1,           0
    nega_loc,       make_nega
            PPD                     ; pad to page 2
    ; page 2
    make_nega,      0               ; subroutine to multiply AC by -1
            CMA IAC                 ; complement and add 1
            JMP     (make_nega)     ; return to caller

Lastly, memory locations 16 through 31 of page 0 are the auto-increment registers. If indirection is used with one of these memory locations, the value contained within is incremented before the operation.

            CLA
            TAD     var1            ; load the value contained at var1
            DCA     16              ; store the value in auto-increment register
            TAD     (16)            ; load the value stored in the address at auto-increment register
            HLT
    var1,           2048

In this code snippet, the value in the AC will be the value located at memory 2049.

### Microinstructions

Microinstructions are specified by the microinstructions keywords. Multiple microinstructions (up to 4) can be declared on the same line, but only microinstructions in the same group. Microinstructions declared together occur in the same clock cycle as each other. If declared together, the precedence of microinstructions is independent of the order they're specified - rather, they act in the order specified on the table below. If a different precedence is required, they must be specified on seperate lines, but they act in separate clock cycles.

Some microinstructions are mutually exclusive, which follows logic. E.g. `SNL` (skip on L != 0) and `SZL` (skip on L = 0) are contradictory, and cannot be specified together.

|Group|Mnemonic|Instruction|Precedence|
|-----|--------|-----------|----------|
|1|CLA|clear AC|1|
|1|CLL|clear L|1|
|1|CMA|ones complement AC|2|
|1|CML|ones complement L|2|
|1|IAC|increment <L, AC>|3|
|1|RAR|rotate <L, AC> right|4|
|1|RAL|rotate <L, AC> left|4|
|1|RTR|rotate <L, AC> right twice|4|
|1|RTL|rotate <L, AC> left twice|4|
|1|BSW|swap AC high order byte with low order byte|4|
|1|NOP|no operation||
|2|SMA|skip on AC < 0|1|
|2|SZA|skip on AC = 0|1|
|2|SNL|skip on L != 0|1|
|2|SKP|unconditional skip||
|2|SPA|skip on AC > 0|1|
|2|SNA|skip on AC != 0|1|
|2|SZL|skip on L = 0|1|
|2|CLA|clear AC after tests|2|
|2|OSR|OR switches with AC||
|2|HLT|Halt operation||

One microinstruction oddity: if you are using CLA, and intend for it to be combined with group 2 microinstructions, make sure it is not the first microinstruction in the line, or the assembler will think it's supposed to be with group 1 instructions.

## Running the assembler

Compile the assembler with `make`, then run via:

`./asm input_file.S`

The assembled binary will be output to the file `a.out`.