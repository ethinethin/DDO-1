#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>

struct sdl2_session {
	struct display {
		int w;
		int h;
		const char *name;
		SDL_Window *window;
		SDL_Renderer *renderer;
	} panel;
	struct display monitor;
	SDL_Texture **font;
	SDL_bool running;
};

#endif
