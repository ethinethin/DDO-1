#ifndef ASM_H
#define ASM_H

#include <stdint.h>

#define PAGE_BIT        0b0000100000000000
#define INDIRECT_BIT    0b0001000000000000

/* Define opcodes, and microinstructions */
struct opcodes {
        const char *code;
        uint16_t binary;
} OPCODES[] = {
        { "AND", 0b0000000000000000 },
        { "TAD", 0b0010000000000000 },
        { "ISZ", 0b0100000000000000 },
        { "DCA", 0b0110000000000000 },
        { "JMS", 0b1000000000000000 },
        { "JMP", 0b1010000000000000 }
};
int N_OPCODES = 6;

struct opcodes GROUP1_OPRS[] = {
        { "CLA", 0b1110100000000000 }, 
        { "CLL", 0b1110010000000000 }, 
        { "CMA", 0b1110001000000000 }, 
        { "CML", 0b1110000100000000 }, 
        { "IAC", 0b1110000000010000 }, 
        { "RAR", 0b1110000010000000 }, 
        { "RAL", 0b1110000001000000 }, 
        { "RTR", 0b1110000010100000 }, 
        { "RTL", 0b1110000001100000 }, 
        { "BSW", 0b1110000000100000 }, 
        { "NOP", 0b1110000000000000 }
};
int N_GROUP1_OPRS = 11;

struct opcodes GROUP2_OPRS[] = {
        { "SMA", 0b1111010000000000 }, 
        { "SZA", 0b1111001000000000 }, 
        { "SNL", 0b1111000100000000 }, 
        { "SKP", 0b1111000010000000 }, 
        { "SPA", 0b1111010010000000 }, 
        { "SNA", 0b1111001010000000 }, 
        { "SZL", 0b1111000110000000 }, 
        { "CLA", 0b1111100000000000 }, 
        { "OSR", 0b1111000001000000 }, 
        { "HLT", 0b1111000000100000 }
};
int N_GROUP2_OPRS = 10;

#endif
