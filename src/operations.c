#include "hardware.h"
#include "operations.h"

/* function prototypes */
static WORDTYPE real_address(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand);
static void     INSTRUCTION_AND(struct ddo1 *cur_ddo1, WORDTYPE address);
static void     INSTRUCTION_TAD(struct ddo1 *cur_ddo1, WORDTYPE address);
static void     INSTRUCTION_ISZ(struct ddo1 *cur_ddo1, WORDTYPE address);
static void     INSTRUCTION_DCA(struct ddo1 *cur_ddo1, WORDTYPE address);
static void     INSTRUCTION_JMS(struct ddo1 *cur_ddo1, WORDTYPE address);
static void     INSTRUCTION_JMP(struct ddo1 *cur_ddo1, WORDTYPE address);
static void     INSTRUCTION_IOT(struct ddo1 *cur_ddo1, WORDTYPE instruction);
static void     INSTRUCTION_OPR(struct ddo1 *cur_ddo1, WORDTYPE instruction);
static void     OPR_GROUP1_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction);
static void     OPR_GROUP2_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction);
static void     rotate(struct ddo1 *cur_ddo1, char direction, int n);
static void     byte_swap(struct ddo1 *cur_ddo1);

void
execute(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{
        WORDTYPE address;
        WORDTYPE indirect;
        WORDTYPE op_code;
        WORDTYPE operand;
        WORDTYPE page;

        /* Determine op code, indirect bit, page bit, and operand */
        op_code = (BITMASK_OPCODE & instruction) >> BITSHIFT_OPCODE;
        indirect = (BITMASK_INDIRECTION & instruction) >> BITSHIFT_INIDRECTION;
        page = (BITMASK_PAGE & instruction) >> BITSHIFT_PAGE;
        operand = (BITMASK_OPERAND & instruction) >> BITSHIFT_OPERAND;
        /* Find the real address that we're accessing */
        address = real_address(cur_ddo1, (uint8_t) indirect, (uint8_t) page, operand);

        /* Call instruction based on op code */
        switch(op_code) {
                case OPCODE_AND:
                        INSTRUCTION_AND(cur_ddo1, address);
                        break;
                case OPCODE_TAD:
                        INSTRUCTION_TAD(cur_ddo1, address);
                        break;
                case OPCODE_ISZ:
                        INSTRUCTION_ISZ(cur_ddo1, address);
                        break;
                case OPCODE_DCA:
                        INSTRUCTION_DCA(cur_ddo1, address);
                        break;
                case OPCODE_JMS:
                        INSTRUCTION_JMS(cur_ddo1, address);
                        break;
                case OPCODE_JMP:
                        INSTRUCTION_JMP(cur_ddo1, address);
                        break;
                case OPCODE_IOT:
                        INSTRUCTION_IOT(cur_ddo1, instruction);
                        break;
                case OPCODE_OPR:
                        INSTRUCTION_OPR(cur_ddo1, instruction);
                        break;
                default:
                        break;
                
        }
}

static WORDTYPE
real_address(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand)
{
        WORDTYPE address;

        /* Are we using page zero or current page? */
        if (page == 0) {
                address = 2047 & operand;
        } else {
                address = (63488 & cur_ddo1->PC) | (2047 & operand);
        }
        /* Are we jumping using indirection? */
        if (indirect == 1) {
                address = cur_ddo1->memory[address];
        }
        return address;
}

static void
INSTRUCTION_AND(struct ddo1 *cur_ddo1, WORDTYPE address)
{
        /* AND the contents of address with the AC, modifying the AC */
        cur_ddo1->AC &= cur_ddo1->memory[address];
} // This passed all tests 2025-01-29

static void
INSTRUCTION_TAD(struct ddo1 *cur_ddo1, WORDTYPE address)
{
        WORDTYPE result;

        /* ADD operand to AC, store carry of the high bit in L */
        result = cur_ddo1->AC + cur_ddo1->memory[address];
        /* check if there is an overflow - only need to check if both values are non-zero */
        if (cur_ddo1->AC > 0 && cur_ddo1->memory[address] > 0) {
                /* Check if you had an overflow */
                if (cur_ddo1->AC > result || cur_ddo1->memory[address] > result) {
                        /* Complement the link bit */
                        cur_ddo1->L = !cur_ddo1->L;
                }
        }
        /* Store the result in the accumulator */
        cur_ddo1->AC = result;
} // This passed all tests 2025-01-29

static void
INSTRUCTION_ISZ(struct ddo1 *cur_ddo1, WORDTYPE address)
{
        /* Increment value in memory address */
        cur_ddo1->memory[address] += 1;
        /* If value is 0, skip the next instruction */
        if (cur_ddo1->memory[address] == 0) cur_ddo1->PC += 1;
}

static void
INSTRUCTION_DCA(struct ddo1 *cur_ddo1, WORDTYPE address)
{
        /* Deposit accumulator in memory address */
        cur_ddo1->memory[address] = cur_ddo1->AC;
        /* Clear accumulator */
        cur_ddo1->AC = 0;
}

static void
INSTRUCTION_JMS(struct ddo1 *cur_ddo1, WORDTYPE address)
{
        /* Store the current address + 1 (return address) in word 0 of the subroutine */
        cur_ddo1->memory[address] = cur_ddo1->PC + 1;
        /* Move to word 0 of the subroutine - this will get incremented *
         * to word 1 before execution of the next instruction           */
        cur_ddo1->PC = address;
}

static void
INSTRUCTION_JMP(struct ddo1 *cur_ddo1, WORDTYPE address)
{
        /* Jump to the address - 1, which will get incremented to the *
         * address before execution of the next instruction           */
        cur_ddo1->PC = address - 1;
} // This passed all tests 2025-01-28

static void
INSTRUCTION_IOT(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{

}

static void
INSTRUCTION_OPR(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{
        /* Mask out non-group bits and check which group the command is for */
        switch(instruction & BITMASK_OPR_GROUPS) {
                case OPR_GROUP1a:
                case OPR_GROUP1b:
                        OPR_GROUP1_HANDLER(cur_ddo1, instruction);
                        break;
                case OPR_GROUP2:
                        OPR_GROUP2_HANDLER(cur_ddo1, instruction);
                        break;
                case OPR_GROUP3:
                        /* I am not implementing group 3 microcodes, so a panic or *
                         * something could be put here if wanted                   */
                        break;
                default:
                        break;
        }
}

static void
OPR_GROUP1_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{
        /* Was NOP called? Just return - no operation necessary */
        if (instruction == GROUP1_NOP) return;
        /* Any of these commands can be called with others, but happen first */
        if ((instruction & GROUP1_CLA) == GROUP1_CLA) cur_ddo1->AC = 0;
        if ((instruction & GROUP1_CLL) == GROUP1_CLL) cur_ddo1->L = 0;
        if ((instruction & GROUP1_CMA) == GROUP1_CMA) cur_ddo1->AC = ~cur_ddo1->AC;
        if ((instruction & GROUP1_CML) == GROUP1_CML) cur_ddo1->L = !cur_ddo1->L;
        if ((instruction & GROUP1_IAC) == GROUP1_IAC) {
                cur_ddo1->AC += 1;
                if (cur_ddo1->AC == 0) cur_ddo1->L = !cur_ddo1->L;
        }
        /* If a rotate is called, check if it's twice before once */
        if ((instruction & GROUP1_RTR) == GROUP1_RTR) {
                rotate(cur_ddo1, 'r', 2);
        } else if ((instruction & GROUP1_RTL) == GROUP1_RTL) {
                rotate(cur_ddo1, 'l', 2);
        } else if ((instruction & GROUP1_RAR) == GROUP1_RAR) {
                rotate(cur_ddo1, 'r', 1);
        } else if ((instruction & GROUP1_RAL) == GROUP1_RAL) {
                rotate(cur_ddo1, 'l', 1);
        } else if ((instruction & GROUP1_BSW) == GROUP1_BSW) {
                byte_swap(cur_ddo1);
        }
}

static void
OPR_GROUP2_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{
        /* Was NOP called? Just return - no operation necessary */
        if (instruction == GROUP2_NOP) return;
        /* Logical tests are done in three groups here - but you have to *
         * check for SPA, SNA, or SZL first because of the bitmasks      */
        if (((instruction & GROUP2_SPA) == GROUP2_SPA && (int16_t) cur_ddo1->AC >= 0) ||
            ((instruction & GROUP2_SNA) == GROUP2_SNA && cur_ddo1->AC != 0) ||
            ((instruction & GROUP2_SZL) == GROUP2_SZL && cur_ddo1->L == 0)) {
                cur_ddo1->PC += 1;
        } else if (((instruction & GROUP2_SMA) == GROUP2_SMA && (int16_t) cur_ddo1->AC < 0) ||
                   ((instruction & GROUP2_SZA) == GROUP2_SZA && cur_ddo1->AC == 0) ||
                   ((instruction & GROUP2_SNL) == GROUP2_SNL && cur_ddo1->L != 0)) {
                cur_ddo1->PC += 1;
        } else if ((instruction & GROUP2_SKP) == GROUP2_SKP) {
                cur_ddo1->PC += 1;
        }

        /* If CLA is called, the AC is cleared after logical tests */
        if ((instruction & GROUP2_CLA) == GROUP2_CLA) cur_ddo1->AC = 0;
        /* I am not going to implement the OSR operation ... at least not yet *
           because it is a "front panel" operation and.. yeah.. you get it    */
        if ((instruction & GROUP2_OSR) == GROUP2_OSR) return;
        /* Was HLT called? Stop operation */
        if ((instruction & GROUP2_HLT) == GROUP2_HLT) cur_ddo1->run = DDO1_OFF;
}

static void
rotate(struct ddo1 *cur_ddo1, char direction, int n)
{
        uint8_t linkbit, leftmostbit, rightmostbit;
        do {
                /* Save values of the link bit, and the edges of the AC */
                linkbit = cur_ddo1->L;
                leftmostbit = ((cur_ddo1->AC & ROT_LEFTMOST_BIT) == ROT_LEFTMOST_BIT) ? 1 : 0;
                rightmostbit = ((cur_ddo1->AC & ROT_RIGHTMOST_BIT) == ROT_RIGHTMOST_BIT) ? 1 : 0;
                /* Shift left or right accordingly */
                if (direction == 'l') {
                        cur_ddo1->AC = cur_ddo1->AC << 1;
                        if (linkbit == 1) cur_ddo1->AC += 1;
                        cur_ddo1->L = leftmostbit;
                } else if (direction == 'r') {
                        cur_ddo1->AC = cur_ddo1->AC >> 1;
                        if (linkbit == 1) cur_ddo1->AC += ROT_LEFTMOST_BIT;
                        cur_ddo1->L = rightmostbit;
                }
                /* Decrement the number of loops remaining */
                n -= 1;
        } while(n > 0);
} // This passed all tests 2025-01-30

static void
byte_swap(struct ddo1 *cur_ddo1)
{
        /* Swap the lower half of the AC with the upper half */
        uint16_t tmp;
        
        /* Put the lower half of the AC in tmp */
        tmp = cur_ddo1->AC & BSW_LOWERBYTE;
        /* Bitshift the upper half down */
        cur_ddo1->AC = cur_ddo1->AC >> BSW_SHIFT;
        /* Bitshift the lower half up in tmp */
        tmp = tmp << BSW_SHIFT;
        /* Combine the numbers together */
        cur_ddo1->AC += tmp;
} // This passed all tests 2025-01-30
