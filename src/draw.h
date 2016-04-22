/*
 * draw.h
 *
 *  Created on: 18 avr. 2016
 *      Author: nathan
 */

#ifndef _DRAW_H_
#define _DRAW_H_

#include <SDL/SDL.h>

void putPixel(SDL_Surface * surface, Uint16 x, Uint16 y, Uint32 color);

void Central_Square (SDL_Surface* hex, int* x, int* y, Uint32 color, int r);

void Triangles (SDL_Surface* hex, int* x, int* y, Uint32 color, int r);

void Hexagon_single (SDL_Surface* hex, int rayon, Uint32 color, int* l);

void Hexagon (SDL_Surface* hex, int rayon, Uint32 color_out, Uint32 color_in, int bord, int* l);

void Circle (SDL_Surface* hex, int l, Uint32 color);

#endif /* _DRAW_H_ */
