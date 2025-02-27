#include <stdio.h>
#include "hardware.h"

/* function prototypes */
static void     trigger_interrupt(struct ddo1 *cur_ddo1);

/* Counter for interrupt enable delay */
static int counter = 0;

void
check_interrupts(struct ddo1 *cur_ddo1)
{
        /* Only continue if interrupts are enabled */
        if (cur_ddo1->intr == INTS_OFF) return;
        /* Only continue if interrupts-reenable counter is >= 5 */
        if (counter < 5) {
                counter += 1;
                return;
        }
        /* Now, trigger interrupts if an interrupt enabled device has         *
         * requested an interrupt                                             */
        if ((cur_ddo1->tty_kbd.intr && cur_ddo1->tty_kbd.flag) ||
            (cur_ddo1->tty_printer.intr && cur_ddo1->tty_printer.flag) ||
            (cur_ddo1->monitor.intr && cur_ddo1->monitor.flag)) {
                trigger_interrupt(cur_ddo1);
        }
}

static void
trigger_interrupt(struct ddo1 *cur_ddo1)
{
        /* Disable interrupts */
        cur_ddo1->intr = INTS_OFF;
        /* Reset interrupts-reenable counter */
        counter = 0;
        /* Save program counter to mem 0 */
        cur_ddo1->memory[0] = cur_ddo1->PC;
        /* Set current operation to mem 1 */
        cur_ddo1->PC = 1;
}
