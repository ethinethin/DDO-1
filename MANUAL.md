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

## Teletype Device

To be written.

## Monitor Device

The monitor device is for outputting text or images to a graphical user display device. The monitor supports two video modes (text and image), and is controlled by sending IOT operations to device 10. Several mnemonics have been added to facilitate monitor control.

### Monitor Commands Table

|Binary|Mnemonic|Short for|Note|Mode|
|---|---|---|---|---|
|1100010100000001|VSW|Video Swap|Swap video mode|Any|
|1100010100000010|VIC|Video Insert Character|Insert character at cursor location|Text|
|1100010100000100|VMC|Video Move Cursor|Move cursor or pixel pointer|Any|
|1100010100001000|VDP|Video Draw Pixel|Change color at pixel pointer|Video|
|1100010100010000|VCR|Video Color Red|Change custom color red|Any|
|1100010100100000|VCG|Video Color Green|Change custom color green value|Any|
|1100010101000000|VCB|Video Color Blue|Change custom color blue value|Any|
|1100010101100000|VCC|Video Color Cyan|Change custom color green and blue values|Any|
|1100010101010000|VCM|Video Color Magenta|Change custom color red and blue values|Any|
|1100010100110000|VCY|Video Color Yellow|Change custom color red and green values|Any|
|1100010101110000|VCK|Video Color Black|Change custom color red, green, and blue values|Any|

All of these are pretty straight forward, but some of them use high order and low order bits to do the work:

### Monitor Commands Explained

1. VSW - simply changes between video modes; the monitor begins in text mode; text mode is made up of 80 columns and 25 rows, while video mode is 240x160 (WxH) 
2. VIC (text mode only) - insert a character, specified by the AC, at current cursor location; lower order byte = character, higher order byte = color
3. VMC - move the cursor or pixel pointer to location specified by the AC; lower order byte = x, higher order byte = y
4. VDP (video mode only) - change color at current pixel pointer to the color number specified by the AC
5. VCR/VCG/VCB - change the red, green, or blue values of the custom color specified by the AC; lower order byte = color value, higher order byte = custom color to change (e.g. 0 would change the first custom color, which is color preset 32)
6. The other color changing commands are combinations of the colors; while specified by mnemonics for the assembler, they are actually microcodes of the other color change operations

# Self-notes, please ignore

...
this is a work in progress, working on adapting my PDP-8 notes to here, also to describe the new features of the DDO-1 vs. the PDP-8

Need to document this somewhere: information about hardware devices. IOT commands are specified by the 110 op code, so the instruction will be:

110DDDDDDIIIIIII

- D = device (up to 64 devices supported)
- I = instruction (up to 128 instructions supported)

I have a teletype keyboard and printer already ready, working via polling but I would like to implement the following:

- Monitor
- Mouse (would certainly need interrupt handling)
- Hard drive
- Hardware interrupts (does a JMS to page 0, mem 0, where the programmer can save the AC, L, then jump to an actual interrupt handler, then reload AC and L before returning)

Technically the PDP-8 memory was persistent between sessions, not random access memory, but this simulator treats it as random access memory and clears it between sessions. But I'd like to put in a hard drive as a device reachable via IOT commands.

