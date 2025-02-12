#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

struct color {
        uint8_t R;
        uint8_t G;
        uint8_t B;
};

extern struct color DDO1_COLORS[];
extern uint8_t N_COLORS;

#endif
