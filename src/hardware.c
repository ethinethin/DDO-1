#include <stdlib.h>
#include "hardware.h"

struct ddo1 *
init_ddo1(void)
{
        int i;
        struct ddo1 *cur_ddo1;

        /* Create a new ddo1 and clear all registers and memory */
        cur_ddo1 = malloc(sizeof(*cur_ddo1));
        cur_ddo1->AC = 0;
        cur_ddo1->L = 0;
        cur_ddo1->PC = 0;
        cur_ddo1->run = DDO1_OFF;
        cur_ddo1->ints = INTS_OFF;
        for (i = 0; i < MEMSIZE; i += 1) {
                cur_ddo1->memory[i] = 0;
        }
        /* Printer is ready */
        cur_ddo1->ttyp_printer.flag = 1;
        return cur_ddo1;
}

void
kill_ddo1(struct ddo1 *cur_ddo1)
{
        free(cur_ddo1);
}

#include <stdio.h>
void
TTY_P_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{
        if (instruction == TTY_TSF && cur_ddo1->ttyp_printer.flag == 1) {
                cur_ddo1->PC += 1;
                return;
        }
        /* Check these individually, because they can be sent together as TLS */
        if ((instruction & TTY_TPC) == TTY_TPC) {
                cur_ddo1->ttyp_printer.flag = 0;
        }
        if ((instruction & TTY_TPC) == TTY_TPC) {
                putchar(cur_ddo1->AC);
        }
        /* Clean up the device after printing - flush stdout and set flag */
        fflush(stdout);
        cur_ddo1->ttyp_printer.flag = 1;
}

void
TTY_K_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{

}
