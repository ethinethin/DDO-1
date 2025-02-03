# Assembler Manual

This is the manual for the DDO-1 assembler. This program and manual are works in progress, and are licensed under the same terms as the DDO-1 project to which they belong.

## Reserved keywords

### Comments

Comments can occur anywhere and are denoted by the character `;`. Any characters following `;` on a line are ignored.

### START and END

`START` is used at the very beginning of every assembly file and END is used at the end. For `START`, specify the memory location that assembly should begin, e.g.:

    START 0         ; Start at memory location 0 (memory location 0 of page 0)
    START 2048      ; Start at memory location 2048 (memory location 0 of page 1)

END is used to signify that assembly is finished. You can include documentation or commentary below this keyword, because nothing after it will be assembled.

### Padding with 0s

`PAD` and `PPD` (page pad) are used to pad zeroes. `PAD` is given a number, to specify how many zeroes are padded, while `PPD` stands alone and means "pad to the next page." For example:

    START 0         ; Start at memory location 0 (memory location 0 of page 0)
    PAD 100         ; pad with 100 zeroes
    CLA             ; this instruction is in memory location 101
    PPD             ; pad until the next page
    CLA             ; this instruction is in memory location 2048

### Labels

Labels are specified with a comma. Lowercase is preferred for labels, and they can be declared in two ways: (1) with a value (to specify the value be stored at the address of the label), or (2) on a line by itself, indicating the label's address me the memory location of the next instruction. Examples:

    var1, 0         ; Store the value 0, var1 refers to this memory location
    loop,           ; A label likely used as a loop point

At assembly time, labels are converted into memory addresses. Label names are lost in the assembled binary.

### Op codes and indirection

The memory reference instruction (MRI) opcodes are given an operand: either a numeric memory address or a label.

|Mnemonic|Operation|
|--------|---------|
|AND|logical and operand and AC|
|TAD|two's complement add operand to <L, AC>|
|ISZ|increment value at address and skip next instruction if result is 0|
|DCA|store AC in address, clear AC|
|JMP|jump to address|
|JMS|jump to subroutine|

The instruction acts on the value stored in memory at that address. If parenthesis are used around the address/label, then the instruction acts on the value stored in memory at the address stored in that memory location instead (indirection). Examples:

    TAD 1000        ; add the value stored at address 1000 to the AC
    TAD var1        ; add the value stored at the label var1 to the AC
    TAD (1000)      ; add the value stored at the address in memory at address 1000
    TAD (var1)      ; add the value stored at the address in memory at label var1

### Microinstructions

Microinstructions are specified by the microinstructions keywords. Multiple microinstructions (up to 4*) can be declared on the same line, but only microinstructions in the same group. Microinstructions declared together occur in the same clock cycle as each other. If declared together, the precedence of microinstructions is independent of the order they're specified - rather, they act in the order specified on the table below. If a different precedence is required, they must be specified on seperate lines, but they act in separate clock cycles. (*note: up to 4 microinstructions not implemented yet, only 2 so far)

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

## Running the assembler

Compile the assembler with `make`, then run via:

`./asm input_file.S`

The output will be `a.bin`.