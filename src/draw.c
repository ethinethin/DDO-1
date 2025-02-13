#include <stdint.h>
#include <SDL2/SDL.h>
#include "draw.h"
#include "font.h"
#include "hardware.h"
#include "main.h"

/* function prototypes */
static void     draw_clear(struct sdl2_session *cur_sdl2);
static void     draw_render(struct sdl2_session *cur_sdl2);
static void     draw_panel(struct sdl2_session *cur_sdl2, struct ddo1 *cur_ddo1);
static void     draw_register(struct sdl2_session *cur_sdl2, WORDTYPE reg, int x, int y);
static void     draw_L(struct sdl2_session *cur_sdl2, uint8_t L, int x, int y);
static void     draw_rect_fill(struct sdl2_session *cur_sdl2, int x, int y, int w, int h, uint8_t fill_c[4], SDL_bool border, uint8_t border_c[4]);
static void     draw_rect_line(struct sdl2_session *cur_sdl2, int x, int y, int w, int h, uint8_t border[4]);
static void     draw_text_mode(struct sdl2_session *cur_sdl2, struct ddo1 *cur_ddo1);
static void     draw_image_mode(struct sdl2_session *cur_sdl2, struct ddo1 *cur_ddo1);
static void     draw_pixel(struct sdl2_session *cur_sdl2, int x, int y, uint8_t color);

/* colors */
static uint8_t BLACK[4] = { 26, 26, 26, 255 };
static uint8_t WHITE[4] = { 255, 255, 255, 255 };
static uint8_t GREEN[4] = { 0, 255, 0, 255 };

void
draw_all(struct sdl2_session *cur_sdl2, struct ddo1 *cur_ddo1)
{
        draw_clear(cur_sdl2);
        draw_panel(cur_sdl2, cur_ddo1);
        if (cur_ddo1->monitor.mode == MON_TEXTMODE) {
                draw_text_mode(cur_sdl2, cur_ddo1);
        } else {
                draw_image_mode(cur_sdl2, cur_ddo1);
        }
        draw_render(cur_sdl2);
}

static void
draw_clear(struct sdl2_session *cur_sdl2)
{
        SDL_SetRenderDrawColor(cur_sdl2->panel.renderer, BLACK[0], BLACK[1], BLACK[2], BLACK[3]);
        SDL_RenderClear(cur_sdl2->panel.renderer);
        SDL_SetRenderDrawColor(cur_sdl2->monitor.renderer, BLACK[0], BLACK[1], BLACK[2], BLACK[3]);
        SDL_RenderClear(cur_sdl2->monitor.renderer);
}

static void
draw_render(struct sdl2_session *cur_sdl2)
{
        SDL_RenderPresent(cur_sdl2->panel.renderer);
        SDL_RenderPresent(cur_sdl2->monitor.renderer);
}

static void
draw_panel(struct sdl2_session *cur_sdl2, struct ddo1 *cur_ddo1)
{
        draw_rect_line(cur_sdl2, 20, 20, 760, 160, WHITE);
        draw_register(cur_sdl2, cur_ddo1->PC, 40, 40);
        draw_L(cur_sdl2, cur_ddo1->L, 40, 80);
        draw_register(cur_sdl2, cur_ddo1->AC, 80, 80);

}

static void
draw_register(struct sdl2_session *cur_sdl2, WORDTYPE reg, int x, int y)
{
        unsigned long int i;
        int bit;

        /* For each bit of the register, light up an LED if it is = 1 */
        for (i = 0; i < (sizeof(reg) * 8); i += 1) {
                bit = reg >> (sizeof(reg) * 8 - i - 1);
                bit = bit & 1;
                if (bit == 1) {
                        draw_rect_fill(cur_sdl2, i * 40 + x, y, 20, 20, GREEN, SDL_TRUE, WHITE);
                } else {
                        draw_rect_line(cur_sdl2, i * 40 + x, y, 20, 20, WHITE);
                }
        }
}

static void
draw_L(struct sdl2_session *cur_sdl2, uint8_t L, int x, int y)
{
        if ((L & 1) == 1) {
                draw_rect_fill(cur_sdl2, x, y, 20, 20, GREEN, SDL_TRUE, WHITE);
        } else {
                draw_rect_line(cur_sdl2, x, y, 20, 20, WHITE);
        }
}

static void
draw_rect_fill(struct sdl2_session *cur_sdl2, int x, int y, int w, int h, uint8_t fill_c[4], SDL_bool border, uint8_t border_c[4])
{
        SDL_Rect rect = { x, y, w, h };
        /* draw the filled rectangle */
        SDL_SetRenderDrawColor(cur_sdl2->panel.renderer, fill_c[0], fill_c[1], fill_c[2], fill_c[3]);
        SDL_RenderFillRect(cur_sdl2->panel.renderer, &rect);
        /* draw the border */
        if (border) {
                draw_rect_line(cur_sdl2, x, y, w, h, border_c);
        }

}

static void
draw_rect_line(struct sdl2_session *cur_sdl2, int x, int y, int w, int h, uint8_t border_c[4])
{
        SDL_Rect rect = { x, y, w, h };
        /* draw the line rectangle */
        SDL_SetRenderDrawColor(cur_sdl2->panel.renderer, border_c[0], border_c[1], border_c[2], border_c[3]);
        SDL_RenderDrawRect(cur_sdl2->panel.renderer, &rect);
}

static void
draw_text_mode(struct sdl2_session *cur_sdl2, struct ddo1 *cur_ddo1)
{
        int i, x, y;
        int color, letter; 

        for (i = 0; i < (80 * 25); i += 1) {
                if ((0xFF & cur_ddo1->monitor.text[i]) != 0) {
                        color = (cur_ddo1->monitor.text[i] >> 8);
                        letter = 0xFF & cur_ddo1->monitor.text[i];
                        /* Convert flat array into 80 x 25 position, then scale to screen */
                        x = i % 80 * FONT_WIDTH * FONT_SCALE_X;
                        y = i / 80 * FONT_HEIGHT * FONT_SCALE_Y;
                        draw_char(cur_sdl2, x, y, letter, color);
                }
        }
}

static void
draw_image_mode(struct sdl2_session *cur_sdl2, struct ddo1 *cur_ddo1)
{
        int i, x, y;
        uint8_t color;

        for (i = 0; i < (240 * 160); i += 1) {
                if ((0xFF & cur_ddo1->monitor.image[i]) != 0) {
                        color = (cur_ddo1->monitor.image[i] >> 8);
                        /* Convert flat array into 240 x 160 position */
                        x = i % 240;
                        y = i / 240;
                        draw_pixel(cur_sdl2, x, y, color);
                }
        }

}

static void
draw_pixel(struct sdl2_session *cur_sdl2, int x, int y, uint8_t color)
{
        /* Pixels are 3.33 x 3.75 */
        SDL_FRect rect = { x * 3.33, y * 3.75, 3.33, 3.75 };
        uint8_t r, g, b;
        /* Set color */	
	r = ((0b11100000 & color) >> 5) * 32;
	g = ((0b00011100 & color) >> 2) * 32;
	b = ((0b00000011 & color) >> 0) * 64;
        SDL_SetRenderDrawColor(cur_sdl2->monitor.renderer, r, g, b, 255);
        /* Draw pixel */
        SDL_RenderFillRectF(cur_sdl2->monitor.renderer, &rect);
        //printf("%d %d %u %u %u\n", x, y, r, g, b);
}
