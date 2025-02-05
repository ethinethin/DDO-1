# DDO-1 Manual

## Introduction

The Dynamic Data Organizer model 1, or DDO-1, is a synthetic, 16-bit computer inspired by the 12-bit DEC PDP-8. This project simulates the artificial hardware, which has these features:

1. Two 16-bit registers (accumulator, AC; and program counter, PC) and one 1-bit register (link bit, L)
2. 65536 16-bit words of random access memory
3. Micro-instruction set (6 memory-requiring instructions, 20+ microcoded operations)
4. Peripherals for user input/output

## Words

The 16-bit words allow 3 bits for op codes, 1 bit for indirection, 1 bit for page specification, and 11 bits for memory addresses. There is no immediate mode, but constants can be declared in memory. The words are laid out like this, from high-to-low order bit:

OOOIZAAAAAAAAAAA

- O = opcode
- I = indirect bit (0 = direct, 1 = indirect)
- Z = page bit (0 = page zero, 1 = current page)
- A = address

### Pages

The 16-bit memory is made up of 32 pages, each consisting of 2048 words. Any given instruction can access memory locations 0-2047 on either the current page (page bit = 1), or page zero (page bit = 0). Jumping to other pages can be accomplished through indirection, by storing the address in an accessible location. For example, if the following address is stored in a word on page 0:

    1110011000000001 ; page 28 (11100), word 1537 (11000000001)

it could be accessed on any page via indirection.

## Instruction Set - memory requiring instructions

There are six memory reference instructions (MRIs). These must all specify an address, may be direct or indirect, and may reference page 0 or the current page.

|Bit code|Mnemonic|Instruction|
|--------|--------|-----------|
| 000    | AND    | AND operand with AC |
| 001    | TAD    | two's complement add operand to <L, AC> |
| 010    | ISZ    | increment operand and skip if result is zero |
| 011    | DCA    | deposit AC in memory and clear AC |
| 100    | JMS    | jump to subroutine |
| 101    | JMP    | jump to operand |
| 110    | IOT    | handle input/output with devices |
| 111    | OPR    | used for microcodes |



...
this is a work in progress, working on adapting my PDP-8 notes to here, also to describe the new features of the DDO-1 vs. the PDP-8