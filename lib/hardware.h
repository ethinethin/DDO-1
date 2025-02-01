#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>

/* definitions for computer properties - changing these may *
 * break the computer                                       */
#define WORDTYPE uint16_t
#define MEMSIZE 65536
#define DDO1_OFF 0
#define DDO1_ON 1
/* definitions for page locations */
#define PAGE_0  0
#define PAGE_1  2048

/* Struct for DDO-1 hardware */
struct ddo1 {
        WORDTYPE AC;
        uint8_t L;
        WORDTYPE PC;
        WORDTYPE memory[MEMSIZE];
        uint8_t run;
};

/* function prototypes */
extern struct ddo1 *    init_ddo1(void);
extern void             kill_ddo1(struct ddo1 *cur_ddo1);

#endif
