#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "hardware.h"

/* definitions for instruction words */
#define BITMASK_OPCODE          0b1110000000000000
#define BITMASK_INDIRECTION     0b0001000000000000
#define BITMASK_PAGE            0b0000100000000000
#define BITMASK_OPERAND         0b0000011111111111
#define BITSHIFT_OPCODE         13
#define BITSHIFT_INIDRECTION    12
#define BITSHIFT_PAGE           11
#define BITSHIFT_OPERAND        0
/* definitions for opcodes */
#define OPCODE_AND              0b000
#define OPCODE_TAD              0b001
#define OPCODE_ISZ              0b010
#define OPCODE_DCA              0b011
#define OPCODE_JMS              0b100
#define OPCODE_JMP              0b101
#define OPCODE_IOT              0b110
#define OPCODE_OPR              0b111
/* definitions for microcode groups */
#define BITMASK_OPR_GROUPS      0b1111000000010000
#define OPR_GROUP1a             0b1110000000000000
#define OPR_GROUP1b             0b1110000000010000
#define OPR_GROUP2              0b1111000000000000
#define OPR_GROUP3              0b1111000000010000
/* definitions for group 1 microcodes */
#define GROUP1_CLA              0b1110100000000000
#define GROUP1_CLL              0b1110010000000000
#define GROUP1_CMA              0b1110001000000000
#define GROUP1_CML              0b1110000100000000
#define GROUP1_IAC              0b1110000000010000
#define GROUP1_RAR              0b1110000010000000
#define GROUP1_RAL              0b1110000001000000
#define GROUP1_RTR              0b1110000010100000
#define GROUP1_RTL              0b1110000001100000
#define GROUP1_BSW              0b1110000000100000
#define GROUP1_NOP              0b1110000000000000
/* definitions for group 2 microcodes */
#define GROUP2_SMA              0b1111010000000000
#define GROUP2_SZA              0b1111001000000000
#define GROUP2_SNL              0b1111000100000000
#define GROUP2_SKP              0b1111000010000000
#define GROUP2_SPA              0b1111010010000000
#define GROUP2_SNA              0b1111001010000000
#define GROUP2_SZL              0b1111000110000000
#define GROUP2_CLA              0b1111100000000000
#define GROUP2_OSR              0b1111000001000000
#define GROUP2_HLT              0b1111000000100000
#define GROUP2_NOP              0b1111000000000000
/* definitions for rotations */
#define ROT_RIGHTMOST_BIT       0b0000000000000001
#define ROT_LEFTMOST_BIT        0b1000000000000000
/* definitions for byte swapping */
#define BSW_LOWERBYTE           0b0000000011111111
#define BSW_SHIFT               8
/* definitions for interrupts */
#define BITMASK_INTS_ON         0b1100000000000001
#define BITMASK_INTS_OFF        0b1100000000000010
/* definitions for hardware */
#define BITMASK_DEVICES         0b0001111110000000

/* function prototypes */
extern void     execute(struct ddo1 *cur_ddo1, WORDTYPE instruction);

#endif
