#ifndef FONT_H
#define FONT_H

#include "main.h"

#define FONT_WIDTH 192
#define FONT_HEIGHT 208
#define FONT_TRANSPARENT 0xFF00FF
/* Fitting 80x25 of these 192x208 into an 800x600 window */
#define FONT_SCALE_X 0.052
#define FONT_SCALE_Y 0.115

extern void     load_font(struct sdl2_session *cur_sdl2);
extern void     unload_font(struct sdl2_session *cur_sdl2);
extern void     draw_char(struct sdl2_session *cur_sdl2, int x, int y, int letter, int color);

#endif
