#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>
#include "colors.h"

/* definitions for computer properties - changing these may *
 * break the computer                                       */
#define WORDTYPE        uint16_t
#define MEMSIZE         65536
#define DDO1_OFF        0
#define DDO1_ON         1
/* definitions for interrupts */
#define INTS_OFF        0
#define INTS_ON         1
/* definitions for page locations */
#define PAGE_0          0
#define PAGE_1          2048
/* definitions for teletype printer: device 4 */
#define TTY_P_DEVICE    0b1100001000000000
#define TTY_TSF         0b1100001000000001
#define TTY_TCF         0b1100001000000010
#define TTY_TPC         0b1100001000000100
/* definitions for teletype keyboard: device 3 */
#define TTY_K_DEVICE    0b1100000110000000
#define TTY_KSF         0b1100000110000001
#define TTY_KCC         0b1100000110000010
#define TTY_KRS         0b1100000110000100
/* Definitions for monitor: device 10 */
#define MON_TEXTSIZE    2000
#define MON_IMAGESIZE   38400
#define MON_TEXTMODE    0
#define MON_IMAGEMODE   1
#define MON_DEVICE      0b1100000110000000
#define MON_VSW         0b1100000110000001
#define MON_VIC         0b1100000110000010
#define MON_VMC         0b1100000110000100
#define MON_VDP         0b1100000110001000
#define MON_VCR         0b1100000110010000
#define MON_VCG         0b1100000110100000
#define MON_VCB         0b1100000111000000

/* Struct for DDO-1 hardware */
struct ddo1 {
        WORDTYPE AC;
        uint8_t L;
        WORDTYPE PC;
        WORDTYPE memory[MEMSIZE];
        uint8_t run;
        uint8_t ints;
        struct tty_printer {
                uint8_t flag;
        } tty_printer;
        struct tty_keyboard {
                uint8_t flag;
                uint8_t n_key;
                int key[20];
        } tty_kbd;
        struct monitor {
                uint8_t mode;
                uint8_t text[MON_TEXTSIZE];
                uint8_t image[MON_IMAGESIZE];
                struct color *colors;
        } monitor;
};

/* function prototypes */
extern struct ddo1 *    init_ddo1(void);
extern void             kill_ddo1(struct ddo1 *cur_ddo1);
extern void             TTY_P_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction);
extern void             TTY_K_HANDLER(struct ddo1 *cur_ddo1, WORDTYPE instruction);
extern void             keypressed(struct ddo1 *cur_ddo1, int keycode);
extern void             keyreleased(struct ddo1 *cur_ddo1, int keycode);

#endif
