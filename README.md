# DDO-1

This project is a simulator for an artificial computer architecture called the Dynamic Data Organizer model 1, or DDO-1. It is a Turing-complete 16-bit computer that uses a small instruction set (6 memory-requiring instructions and a handful of bit-coded micro-instructions).

It has two 16-bit registers (AC and PC), a  1-bit register (L, which functions as a carry flag), 65,535 16-bit words of random access memory, and some peripherals.

Words 16-31 on page 0 function as auto-increment registers. When one of these words is used as an indirect address pointer, its stored value is incremented before being used as the effective address.

A majority of the instructions are in place but I am still testing and validating them, mostly by writing programs. Right now, the panel displayed the PC (top row), and the L + AC (bottom row).

A basic, but functional, assembler is located in the ./assembler directory. It is a work in progress, though improving daily. See the details in the ./assembler/README.md and ./assembler/MANUAL.md files.

Example programs are in the ./programs directory. These contain programs that will assemble and run in the simulator. They demonstrate different capabilities of the simulator as well as the assembly style.

I am working on implementing: hardware peripherals, interrupt handling, etc.
