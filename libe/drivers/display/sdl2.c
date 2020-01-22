/*
 * Display driver: SDL2
 */

#if defined(USE_DISPLAY) && defined(USE_SDL2)

#include <SDL2/SDL.h>
#include <libe/libe.h>


int8_t display_sdl2_open(struct display *display, void *context, uint8_t address, int16_t w, int16_t h)
{
	memset(display, 0, sizeof(*display));

	display->w = w ? w : 128;
	display->h = h ? h : 64;
	display->opt = display_sdl2_opt;
	display->pixel = display_sdl2_pixel;
	display->scaling = 1;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(display->w, display->h, 0, (SDL_Window **)&display->context, (SDL_Renderer **)&display->buffer);
	SDL_RenderClear((SDL_Renderer *)display->buffer);
	SDL_RenderPresent((SDL_Renderer *)display->buffer);

	return 0;
}

int32_t display_sdl2_opt(struct display *display, struct opt *opt)
{
	switch (opt->opt) {
	case DISPLAY_OPT_GET_W:
		opt->i16 = display->w;
		return 0;
	case DISPLAY_OPT_GET_H:
		opt->i16 = display->h;
		return 0;
	case DISPLAY_OPT_SET_SCALING:
		display->scaling = opt->u8;
		SDL_SetWindowSize((SDL_Window *)display->context, display->w * display->scaling, display->h * display->scaling);
		SDL_DestroyRenderer((SDL_Renderer *)display->buffer);
		display->buffer = (uint8_t *)SDL_CreateRenderer((SDL_Window *)display->context, -1, 0);
		SDL_RenderClear((SDL_Renderer *)display->buffer);
		SDL_RenderPresent((SDL_Renderer *)display->buffer);
		return 0;
	}

	return -1;
}

void display_sdl2_pixel(struct display *display, int16_t x, int16_t y, uint32_t color)
{
	SDL_Renderer *renderer = (SDL_Renderer *)display->buffer;
	SDL_Rect rc = { x * display->scaling, y * display->scaling, display->scaling, display->scaling };
	SDL_SetRenderDrawColor(renderer, color & 0xff, (color >> 8) & 0xff, (color >> 16) & 0xff, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rc);
	SDL_RenderPresent(renderer);
}

#endif
