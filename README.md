# DDO-1

This project is a simulator for an artificial computer architecture called the
Dynamic Data Organizer model 1, or DDO-1. It is a Turing-complete 16-bit computer
that uses a small instruction set (6 memory-requiring instructions and a handful
of bit-coded micro-instructions). It has two 16-bit registers (AC and PC), a 
1-bit register (L, which functions as a carry flag), 65,535 16-bit words of
random access memory, and some peripherals. 

Implemented so far:

A majority of the instructions are in place and I have tested and validated
about half of them. Right now, the panel displayed the PC (top row), and the
L + AC (bottom row). I need to implement a way to load programs from a file. I
also need to write an assembler so I don't have to manually assemble files.