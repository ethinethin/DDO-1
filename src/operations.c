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
                        if (cur_ddo1->L == 0) {
                                cur_ddo1->L = 1;
                        } else {
                                cur_ddo1->L = 0;
                        }
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
        /* Store the current address in word 0 of the subroutine */
        cur_ddo1->memory[address] = cur_ddo1->PC;
        /* Move to word 1 of the subroutine */
        cur_ddo1->PC = address + 1;
}

static void
INSTRUCTION_JMP(struct ddo1 *cur_ddo1, WORDTYPE address)
{
        /* Jump to the address */
        cur_ddo1->PC = address;
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
                case OPR_GROUP1:
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
        
}

static void
OPR_GROUP2_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{
        
}
