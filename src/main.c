#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "draw.h"
#include "hardware.h"
#include "main.h"
#include "memio.h"
#include "operations.h"

/* Render every X operations */
#define RENDER_EVERY 50

/* Function prototypes */
static struct sdl2_session *	init_sdl2(int w, int h, const char *name);
static void			kill_sdl2(struct sdl2_session *cur_sdl2);
static void			init_disp(struct sdl2_session *cur_sdl2);
static void			kill_disp(struct sdl2_session *cur_sdl2);

int
main(int argc, char *argv[])
{
	SDL_Event event;
	struct sdl2_session *cur_sdl2;
	struct ddo1 *cur_ddo1;
	int counter;
	uint16_t instruction;
	
	/* Set up current sdl2 session */
	cur_sdl2 = init_sdl2(800, 200, "DDO-1");
	/* Set up the current computer */
	cur_ddo1 = init_ddo1();
	/* Try to load memory from the file - this will activate the computer */
	if (argc == 2) load_memory(cur_ddo1, argv[1]);
	counter = 0;
	/* Enter main loop */
	while (cur_sdl2->running == SDL_TRUE) {
		/* If the computer is running, execute instructions */
		if (cur_ddo1->run == DDO1_ON) {
			/* fetch the instruction, increment PC, then execute */
			instruction = cur_ddo1->memory[cur_ddo1->PC];
			cur_ddo1->PC += 1;
			execute(cur_ddo1, instruction);
			/* redraw if the computer was halted */
			if (cur_ddo1->run == DDO1_OFF) {
				draw_all(cur_sdl2, cur_ddo1);
			}
		} else {
			/* Computer is halted, so delay 50 ms to save CPU cycles */
			SDL_Delay(50);
		}
		/* Don't draw after every operation - only after the number of *
		 * instructions defined above  				       */
		if (counter % RENDER_EVERY == 0) draw_all(cur_sdl2, cur_ddo1);
		counter += 1;
		/* Check for event and loop */
		if (SDL_PollEvent(&event) == 0) continue;
		if (event.type == SDL_QUIT) {
			cur_sdl2->running = SDL_FALSE;
		} else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				cur_sdl2->running = SDL_FALSE;
				break;
			}
			/* Add to key queue */
			keypressed(cur_ddo1, event.key.keysym.sym);
		} else if (event.type == SDL_KEYUP) {
			/* Remove from key queue */
			keyreleased(cur_ddo1, event.key.keysym.sym);
		}
	}
	/* Kill current computer and sdl2 session */
	kill_ddo1(cur_ddo1);
	kill_sdl2(cur_sdl2);
	return 0;
}

static struct sdl2_session *
init_sdl2(int w, int h, const char *name)
{
	struct sdl2_session *cur_sdl2;

	/* Set up the sdl2 session */
	cur_sdl2 = malloc(sizeof(*cur_sdl2));
	cur_sdl2->display.w = w;
	cur_sdl2->display.h = h;
	cur_sdl2->display.name = name;
	cur_sdl2->display.window = NULL;
	cur_sdl2->display.renderer = NULL;
	cur_sdl2->running = SDL_FALSE;
	/* Initialize SDL and display */
	SDL_Init(SDL_INIT_EVERYTHING);
	init_disp(cur_sdl2);
	return cur_sdl2;
}

static void
kill_sdl2(struct sdl2_session *cur_sdl2)
{
	/* Destroy display, quit SDL, and free the sdl2 session */
	kill_disp(cur_sdl2);
	free(cur_sdl2);
}



static void
init_disp(struct sdl2_session *cur_sdl2)
{
	SDL_Event event;
	
	cur_sdl2->display.window = SDL_CreateWindow(cur_sdl2->display.name, 0, 0, cur_sdl2->display.w, cur_sdl2->display.h, SDL_WINDOW_SHOWN);
	cur_sdl2->display.renderer = SDL_CreateRenderer(cur_sdl2->display.window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE|SDL_RENDERER_PRESENTVSYNC);
	// You may want this
	//SDL_SetRenderDrawBlendMode(cur_sdl2->display.renderer, SDL_BLENDMODE_BLEND);
	/* Create output texture */
	/* Poll for an event and wait 100ms - bring the window to the foreground */
	SDL_PollEvent(&event);
	SDL_Delay(100);
	/* Game is running */
	cur_sdl2->running = SDL_TRUE;
}

static void
kill_disp(struct sdl2_session *cur_sdl2)
{
	/* Delay to prevent segfaults in other thread loops */
	SDL_Delay(100);
	SDL_DestroyRenderer(cur_sdl2->display.renderer);
	SDL_DestroyWindow(cur_sdl2->display.window);
	SDL_Quit();
}
