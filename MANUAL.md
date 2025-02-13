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

|Binary|Mnemonic|Short for|Note|
|---|---|---|---|
|1100010100000001|VSF|Video Skip if Flag|Skip if flag is set|
|1100010100000010|VMS|Video Mode Set|Change current video mode|
|1100010100000100|VRC|Video Render Color|Change current color|
|1100010100001000|VMC|Video Move Cursor|Move the location of the text/drawing cursor|
|1100010100010000|VDC|Video Draw at Cursor|Output character or pixel at cursor location|

These are pretty straight forward, but how they work is described below.

### Monitor Commands Explained

1. VSF - skips the next command if the monitor ready flag is set. When the monitor can receive input, the ready flag will be enabled. If the flag is enabled, this operation will skip the next operation.
2. VMS - sets the display mode. The monitor begins in text mode. When this operation executes, it will change the display mode to text mode (if AC = 0) or image mode (if AC != 0). When switching video modes, the device is re-initialized, clearing the contents of the screen. So calling a "switch to text mode" when already in text mode is a fast way to clear the screen, etc.
3. VRC - sets the current drawing color. This switches the current drawing color using the contents of the AC. The color format used is RGB332 (i.e. 3-bits for red, 3-bits for green, and 2-bits for blue).
4. VDC - this draws at the current cursor location. If in text mode, the 8-bit ASCII character in the lower byte of the AC will be drawn at the cursor location, in the color specified by VSC. If in image mode, a pixel will be drawn at the current pixel location. After the operation, the cursor location will be increased by one. 
5. VMC - move the cursor or pixel pointer to location specified by the AC; lower order byte = x, higher order byte = y


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

