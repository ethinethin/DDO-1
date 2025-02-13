#include <SDL2/SDL.h>
#include "font.h"
#include "main.h"

/* function prototypes */
static int      convert_letter(int letter);

void
load_font(struct sdl2_session *cur_sdl2)
{
	int i, j;
	SDL_Surface *image, *tmp;
	SDL_Rect rect = {0, 0, FONT_WIDTH, FONT_HEIGHT};

	/* Allocate memory for 95 font characters */
	cur_sdl2->font = malloc(sizeof(*cur_sdl2->font)*95);
	/* Load sprite sheet */
	image = SDL_LoadBMP("art/font.bmp");
	/* Load all sprites */
	for (i = 0; i < 10; i += 1) {
		for (j = 0; j < 10; j += 1) {
			if (i * 10 + j == 95) break;
			rect.x = j * FONT_WIDTH;
			rect.y = i * FONT_HEIGHT;
			tmp = SDL_CreateRGBSurface(0, FONT_WIDTH, FONT_HEIGHT, 24, 0x00, 0x00, 0x00, 0x00);
			SDL_SetColorKey(tmp, 1, FONT_TRANSPARENT);
			SDL_BlitSurface(image, &rect, tmp, NULL);
			cur_sdl2->font[i * 10 + j] = SDL_CreateTextureFromSurface(cur_sdl2->monitor.renderer, tmp);
			SDL_SetTextureBlendMode(cur_sdl2->font[i * 10 + j], SDL_BLENDMODE_BLEND);
			SDL_FreeSurface(tmp);		
		}
	}
	SDL_FreeSurface(image);
}

void
unload_font(struct sdl2_session *cur_sdl2)
{
	int i;

	/* Free all font characters */
	for (i = 0; i < 95; i++) {
		SDL_DestroyTexture(cur_sdl2->font[i]);
	}
	free(cur_sdl2->font);
}

void
draw_char(struct sdl2_session *cur_sdl2, int x, int y, int letter, uint8_t color)
{
	SDL_Rect rect = { x, y, FONT_WIDTH * FONT_SCALE_X, FONT_HEIGHT * FONT_SCALE_Y };
	uint8_t r, g, b;
        /* Adjust to correct character from ASCII */
        letter = convert_letter(letter);
	/* Translate 8-bit color */
	r = ((0b11100000 & color) >> 5) * 32;
	g = ((0b00011100 & color) >> 2) * 32;
	b = ((0b00000011 & color) >> 0) * 64;
	
        /* Change color, render, then change color back */
        SDL_SetTextureColorMod(cur_sdl2->font[letter], r, g, b);
	SDL_RenderCopyEx(cur_sdl2->monitor.renderer, cur_sdl2->font[letter], NULL, &rect, 0, NULL, 0);
        SDL_SetTextureColorMod(cur_sdl2->font[letter], 255, 255, 255);
}

static int
convert_letter(int letter)
{
        /* The font values are basically ASCII - 32 */
        if (letter >= 32) {
                return letter - 32;
        } else {
                /* Return a space otherwise */
                return 0;
        }
}
