/*
 * draw.c
 *
 *  Created on: 18 avr. 2016
 *      Author: nathan
 */

#include "draw.h"

#include <math.h>

#include "globals.h"


/* source : http://www.gnurou.org/writing/linuxmag/sdl/partie1 */
void putPixel(SDL_Surface * surface, Uint16 x, Uint16 y, Uint32 color)
{
    /* Nombre de bits par pixels de la surface d'écran */
    Uint8 bpp = surface->format->BytesPerPixel;
    /* Pointeur vers le pixel à remplacer (pitch correspond à la taille
       d'une ligne d'écran, c'est à dire (longueur * bitsParPixel)
       pour la plupart des cas) */
    Uint8 * p = ((Uint8 *)surface->pixels) + y * surface->pitch + x * bpp;
     switch(bpp)
    {
	  case 1:
		*p = (Uint8) color;
		break;
        case 2:
            *(Uint16 *)p = (Uint16) color;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                *(Uint16 *)p = ((color >> 8) & 0xff00) | ((color >> 8) & 0xff);
                *(p + 2) = color & 0xff;
            }
            else
            {
                *(Uint16 *)p = color & 0xffff;
                *(p + 2) = ((color >> 16) & 0xff) ;
            }
            break;
        case 4:
            *(Uint32 *)p = color;
            break;
    }
}

void Central_Square (SDL_Surface* hex, int* x, int* y, Uint32 color, int r)
{
	int width = x[0] - x[2];
	int height = r;//y[2] - y[3];

	SDL_Surface* square = SDL_CreateRGBSurface (SDL_HWSURFACE, width, height, 32, 0, 0, 0, 0);
	SDL_FillRect (square, NULL, color);
	SDL_Rect position = {x[3], y[3]};
	SDL_BlitSurface (square, NULL, hex, &position);
	SDL_FreeSurface (square);
}

void Triangles (SDL_Surface* hex, int* x, int* y, Uint32 color, int r)
{
	int h = r/2;//y[1] - y[2];
	int l = (x[0] - x[2])/2;
	SDL_LockSurface (hex);
	for (int j = 0; j < h; ++j)
		for (int i = - l * (h - j) / h; i <= l * (h - j) / h; ++i)
		{
			putPixel (hex, x[1] + i - 1, y[3] - j, color);
			putPixel (hex, x[1] + i - 1, y[0] + j, color);
		}
	SDL_UnlockSurface (hex);
}

void Hexagon (SDL_Surface* hex, int rayon, Uint32 color_out, Uint32 color_in, int bord, int* l)
{
	int x [4];
	int y [4];
	for (int i = 0; i < 4; ++i)
	{
		x [i] = cos(i * PI/3 + PI/6) * rayon + rayon;
		y [i] = sin(i * PI/3 + PI/6) * rayon + rayon;
	}
	*l = x[0] - x[2];
	Central_Square (hex, x, y, color_out, rayon);
	Triangles (hex, x, y, color_out, rayon);
	for (int i = 0; i < 4; ++i)
	{
		x [i] = cos(i * PI/3 + PI/6) * (rayon - bord) + rayon;
		y [i] = sin(i * PI/3 + PI/6) * (rayon - bord) + rayon;
	}
	Central_Square (hex, x, y, color_in, rayon - bord);
	Triangles (hex, x, y, color_in, rayon - bord);
	SDL_Flip (hex);
	SDL_SetColorKey (hex , SDL_SRCCOLORKEY, SDL_MapRGB(hex->format, 0, 0, 0)); // set black as transparent
}

void Circle (SDL_Surface* hex, int l, Uint32 color)
{
	int r = (l - 6) / 2.5;
	SDL_LockSurface (hex);
	for (float j = 0; j < 2 * PI; j = j + 1. / (2*r))
		for (int i = 0; i < r; ++i)
			putPixel (hex, i * cos (j) + hex->h / 2 - 1, i * sin (j) + hex->h / 2, color);
	SDL_UnlockSurface (hex);
}
