#include "hardware.h"

static void    INSTRUCTION_AND(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand);
static void    INSTRUCTION_TAD(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand);
static void    INSTRUCTION_ISZ(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand);
static void    INSTRUCTION_DCA(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand);
static void    INSTRUCTION_JMS(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand);
static void    INSTRUCTION_JMP(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand);

void
execute(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{
        WORDTYPE op_code;
        WORDTYPE indirect;
        WORDTYPE page;
        WORDTYPE operand;

        /* Determine op code, indirect bit, page bit, and operand */
        op_code = (57344 & instruction) >> 13;
        indirect = (4096 & instruction) >> 12;
        page = (2048 & instruction) >> 11;
        operand = 2047 & instruction;
        /* Call instruction based on op code */
        switch(op_code) {
                case 0:
                        INSTRUCTION_AND(cur_ddo1, (uint8_t) indirect, (uint8_t) page, operand);
                        break;
                case 1:
                        INSTRUCTION_TAD(cur_ddo1, (uint8_t) indirect, (uint8_t) page, operand);
                        break;
                case 2:
                        INSTRUCTION_ISZ(cur_ddo1, (uint8_t) indirect, (uint8_t) page, operand);
                        break;
                case 3:
                        INSTRUCTION_DCA(cur_ddo1, (uint8_t) indirect, (uint8_t) page, operand);
                        break;
                case 4:
                        INSTRUCTION_JMS(cur_ddo1, (uint8_t) indirect, (uint8_t) page, operand);
                        break;
                case 5:
                        INSTRUCTION_JMP(cur_ddo1, (uint8_t) indirect, (uint8_t) page, operand);
                        break;
                default:
                        break;
                
        }
}

static void
INSTRUCTION_AND(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand) {

}

static void
INSTRUCTION_TAD(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand) {
        
}

static void
INSTRUCTION_ISZ(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand) {
        
}

static void
INSTRUCTION_DCA(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand) {
        
}

static void
INSTRUCTION_JMS(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand) {
        
}

static void
INSTRUCTION_JMP(struct ddo1 *cur_ddo1, uint8_t indirect, uint8_t page, WORDTYPE operand) {
        WORDTYPE address;

        /* Are we using page zero or current page? */
        if (page == 0) {
                /* We are using page 0 so real address is just the    *
                 * right-most 11 bits of the operand (2047 & operand) */
                address = 2047 & operand;
        } else {
                /* We are using current page, so real address is the *
                 * left-most 5 bits of PC and the right-most 11 bits *
                 * of the operand (63488 & PC) | (2047 & operand) */
                address = (63488 & cur_ddo1->PC) | (2047 & operand);
        }

        /* Are we jumping directly or using indirection? */
        if (indirect == 0) {
                cur_ddo1->PC = address;
        } else {
                cur_ddo1->PC = cur_ddo1->memory[address];
        }
}
