#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "colors.h"
#include "hardware.h"

/* function prototypes */
static int      shift_fix(int keycode);

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
        /* Initialize printer - flag on because it's ready */
        cur_ddo1->tty_printer.flag = 1;
        /* Initialize keyboard - flag off because no keys are pressed */
        cur_ddo1->tty_kbd.flag = 0;
        /* Zero keys pressed */
        cur_ddo1->tty_kbd.n_key = 0;
        for (i = 0; i < 10; i += 1) {
                cur_ddo1->tty_kbd.key[i] = 0;
        }
        /* Initialize monitor - set text mode, zero out display, and init colors */
        cur_ddo1->monitor.mode = MON_TEXTMODE;
        for (i = 0; i < MON_IMAGESIZE; i += 1) {
                if (i < MON_TEXTSIZE) cur_ddo1->monitor.text[i] = 0;
                cur_ddo1->monitor.image[i] = 0;
        }
        cur_ddo1->monitor.colors = DDO1_COLORS;
        return cur_ddo1;
}

void
kill_ddo1(struct ddo1 *cur_ddo1)
{
        free(cur_ddo1);
}

void
TTY_P_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{
        /* If flag skip is called and flag is set, skip and return */
        if (instruction == TTY_TSF && cur_ddo1->tty_printer.flag == 1) {
                cur_ddo1->PC += 1;
                return;
        }
        /* Check these individually, because they can be sent together as TLS */
        if ((instruction & TTY_TPC) == TTY_TPC) {
                /* clear flag instruction */
                cur_ddo1->tty_printer.flag = 0;
        }
        if ((instruction & TTY_TPC) == TTY_TPC) {
                putchar(cur_ddo1->AC);
        }
        /* Clean up the device after printing - flush stdout and set flag */
        fflush(stdout);
        cur_ddo1->tty_printer.flag = 1;
}

void
TTY_K_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction)
{
        /* If flag skip is called and flag is set, skip and return */
        if (instruction == TTY_KSF && cur_ddo1->tty_kbd.flag == 1) {
                cur_ddo1->PC += 1;
                return;
        }
        /* Check these individual, because they can be sent together as KRB */
        if ((instruction & TTY_KCC) == TTY_KCC) {
                /* clear flag instruction */
                cur_ddo1->tty_kbd.flag = 0;
        }
        /* This should only return something if there is information in the  *
         * buffer. If there is nothing, key[0] will be equal to 0            */
        if ((instruction & TTY_KRS) == TTY_KRS) {
                /* read key in buffer */
                cur_ddo1->AC |= cur_ddo1->tty_kbd.key[0];
                /* remove key from buffer */
                keyreleased(cur_ddo1, cur_ddo1->tty_kbd.key[0]);
        }

}

int LSHIFT = 0;
int RSHIFT = 0;
void
keypressed(struct ddo1 *cur_ddo1, int keycode)
{
        int i;

        /* If shift, enable shift */
        if (keycode == SDLK_LSHIFT) {
                LSHIFT = 1;
                return;
        } else if (keycode == SDLK_RSHIFT) {
                RSHIFT = 1;
                return;
        }
        /* If shift is held in, fix the keycode */
        if (LSHIFT == 1 || RSHIFT == 1) {
                keycode = shift_fix(keycode);
        }
        if (keycode == '\r') keycode = '\n';

        /* Make sure not over the keypress limit */
        if (cur_ddo1->tty_kbd.n_key >= 20) return;
        /* Check if it's already pressed (shouldn't be) */
        for (i = 0; i < cur_ddo1->tty_kbd.n_key; i += 1) {
                if (cur_ddo1->tty_kbd.key[i] == keycode) return;
        }
        /* Add to the key queue */
        cur_ddo1->tty_kbd.key[i] = keycode;
        cur_ddo1->tty_kbd.n_key += 1;
        /* Turn on keyboard flag if not on */
        cur_ddo1->tty_kbd.flag = 1;
}

static int
shift_fix(int keycode)
{
        /* If it's between 97-122, subtract 32 */
        if (keycode >= 97 && keycode <= 122) return keycode - 32;

        /* It is something else */
        switch (keycode) {
                case '1': return '!';
                case '2': return '@';
                case '3': return '#';
                case '4': return '$';
                case '5': return '%';
                case '6': return '^';
                case '7': return '&';
                case '8': return '*';
                case '9': return '(';
                case '0': return ')';
                case '-': return '_';
                case '=': return '+';
                case '[': return '{';
                case ']': return '}';
                case ';': return ':';
                case '\'': return '"';
                case ',': return '<';
                case '.': return '>';
                case '/': return '?';
                case '\\': return '|';
                case '`':  return '~';
                default: return keycode;
        }
}

void
keyreleased(struct ddo1 *cur_ddo1, int keycode)
{
        int i, j;

        /* If shift, disable shift */
        if (keycode == SDLK_LSHIFT) {
                LSHIFT = 0;
                return;
        } else if (keycode == SDLK_RSHIFT) {
                RSHIFT = 0;
                return;
        }
        /* Find the key in the queue */
        for (i = 0; i < cur_ddo1->tty_kbd.n_key; i += 1) {
                if (cur_ddo1->tty_kbd.key[i] == keycode) {
                        /* Remove it from the queue */
                        cur_ddo1->tty_kbd.key[i] = 0;
                        /* If there are more after it, move them backward one position */
                        if (i < cur_ddo1->tty_kbd.n_key - 1) {
                                for (j = i + 1; j < cur_ddo1->tty_kbd.n_key; j += 1) {
                                        cur_ddo1->tty_kbd.key[j - 1] = cur_ddo1->tty_kbd.key[j];
                                }
                        }
                        /* Reduce number of keys in the queue */
                        cur_ddo1->tty_kbd.n_key -= 1;
                        /* Manage the keyboard flag */
                        if (cur_ddo1->tty_kbd.n_key > 0) {
                                cur_ddo1->tty_kbd.flag = 1;
                        } else {
                                cur_ddo1->tty_kbd.flag = 0;
                        }
                        break;
                }
        }
}
