#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <SDL/SDL.h>

SDL_Surface* init_window ();

SDL_Surface* resize_window (SDL_Surface* window, SDL_Event* event);

SDL_Surface* fullscreen_window (SDL_Surface* window);

void Reset_window (SDL_Surface* window);

void Free_window (SDL_Surface* window);

#endif /* _WINDOW_H_ */
