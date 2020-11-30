/*
 * Display driver: SDL2
 */

#if defined(USE_DISPLAY) && defined(USE_DRIVER_SDL2)

#include <SDL2/SDL.h>
#include <libe/libe.h>


int8_t display_sdl2_open(struct display *display, void *context, uint8_t address, int16_t w, int16_t h)
{
	memset(display, 0, sizeof(*display));

	display->w = w ? w : 128;
	display->h = h ? h : 64;
	display->clip_x1 = 0;
	display->clip_y1 = 0;
	display->clip_x2 = display->w - 1;
	display->clip_y2 = display->h - 1;

	display->close = display_sdl2_close;
	display->opt = display_sdl2_opt;
	display->pixel = display_sdl2_pixel;
	display->scaling = 1;

	display->hline = draw_generic_hline;
	display->vline = draw_generic_vline;
	display->rect = draw_generic_rect;
	display->fill = draw_generic_fill;

	display->update = display_sdl2_update;

	SDL_Init(SDL_INIT_VIDEO);
	display->context = (SDL_Window *)SDL_CreateWindow("libe sdl2 test window", 100, 100, display->w * display->scaling, display->h * display->scaling, 0);
	display->buffer = (uint8_t *)SDL_CreateRenderer((SDL_Window *)display->context, -1, 0);
	SDL_SetRenderDrawColor((SDL_Renderer *)display->buffer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear((SDL_Renderer *)display->buffer);
	SDL_RenderPresent((SDL_Renderer *)display->buffer);

	return 0;
}

void display_sdl2_close(struct display *display)
{
	SDL_DestroyRenderer((SDL_Renderer *)display->buffer);
	SDL_DestroyWindow((SDL_Window *)display->context);
	SDL_Quit();
}

int8_t display_sdl2_opt(struct display *display, uint8_t opt, void *value)
{
	switch (opt) {
	case DISPLAY_OPT_GET_W:
		(*((int16_t *)value)) = display->w;
		return 0;
	case DISPLAY_OPT_GET_H:
		(*((int16_t *)value)) = display->h;
		return 0;
	case DISPLAY_OPT_SET_SCALING:
		display->scaling = (*((float *)value)) > 0 ? (*((float *)value)) : 1;
		SDL_SetWindowSize((SDL_Window *)display->context, display->w * display->scaling, display->h * display->scaling);
		SDL_DestroyRenderer((SDL_Renderer *)display->buffer);
		display->buffer = (uint8_t *)SDL_CreateRenderer((SDL_Window *)display->context, -1, 0);
		SDL_SetRenderDrawColor((SDL_Renderer *)display->buffer, 0, 0, 0, SDL_ALPHA_OPAQUE);
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

	SDL_Event event;
	while (SDL_PollEvent(&event));
}

void display_sdl2_update(struct display *display)
{
	SDL_Renderer *renderer = (SDL_Renderer *)display->buffer;
	SDL_RenderPresent(renderer);
	SDL_Event event;
	while (SDL_PollEvent(&event));
}

#endif
