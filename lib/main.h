#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>

struct sdl2_session {
	struct {
		int w;
		int h;
		const char *name;
		SDL_Window *window;
		SDL_Renderer *renderer;
	} display;
	SDL_bool running;
};

#endif
