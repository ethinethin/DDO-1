#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "hardware.h"

/* definitions for instruction words */
#define BITMASK_OPCODE 57344
#define BITMASK_INDIRECTION 4096
#define BITMASK_PAGE 2048
#define BITMASK_OPERAND 2047
#define BITSHIFT_OPCODE 13
#define BITSHIFT_INIDRECTION 12
#define BITSHIFT_PAGE 11
#define BITSHIFT_OPERAND 0
/* definitions for opcodes */
#define OPCODE_AND 0
#define OPCODE_TAD 1
#define OPCODE_ISZ 2
#define OPCODE_DCA 3
#define OPCODE_JMS 4
#define OPCODE_JMP 5
#define OPCODE_IOT 6
#define OPCODE_OPR 7
/* definitions for microcode groups */
#define BITMASK_OPR_GROUPS 61441
#define OPR_GROUP1 57344
#define OPR_GROUP2 61440
#define OPR_GROUP3 61441
/* definitions for group 1 microcodes */
#define GROUP1_CLA 59392
#define GROUP1_CLL 58368
#define GROUP1_CMA 57856
#define GROUP1_CML 57600
#define GROUP1_IAC 57345
#define GROUP1_RAR 57472
#define GROUP1_RAL 57408
#define GROUP1_RTR 57504
#define GROUP1_RTL 57440
#define GROUP1_BSW 57376
#define GROUP1_NOP 57344
/* definitions for group 2 microcodes */
#define GROUP2_SMA
#define GROUP2_SZA
#define GROUP2_SNL
#define GROUP2_SKP
#define GROUP2_SPA
#define GROUP2_SNA
#define GROUP2_SZL
#define GROUP2_CLA
#define GROUP2_OSR
#define GROUP2_HLT
#define GROUP2_NOP

/* function prototypes */
extern void     execute(struct ddo1 *cur_ddo1, WORDTYPE instruction);

#endif
