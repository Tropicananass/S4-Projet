#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL.h>
#include <SDL_gfxPrimitives.h>
//#include <SDL_image.h>
//#include <SDL_rotozoom.h>
//#include <SDL_ttf.h>

#define WIDTH 1200
#define HEIGHT 700
#define NBSIDE 11
#define COLOR 32
#define PI 3.14159265

#define SQ(a) a * a
#define PWIDTH(i, r) r/2 + i * (1.5 * r + 1)
#define PHEIGHT(i, r) (i + (i - 1) / 2.) * (2 * r + cos(PI/6) + 1)

//gcc -Wall -std=c99 `sdl-config --cflags --libs` -g -lSDL_image -lSDL_gfx -lm -lSDL_ttf -o plateau plateau.c

typedef struct vector_s {
	int x, y;
} vector_t;

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void Central_Square (SDL_Surface* window, Sint16* x, Sint16* y, Uint32 color)
{
	int width = x[0] - x[2];
	int height = y[2] - y[3];
	
	SDL_Surface* square = SDL_CreateRGBSurface (SDL_HWSURFACE, width, height, 32, 0, 0, 0, 0);
	SDL_FillRect (square, NULL, color);
	SDL_Rect position = {x[3], y[3]};
	SDL_BlitSurface (square, NULL, window, &position);
	SDL_FreeSurface (square);
}

void Triangles (SDL_Surface* window, Sint16* x, Sint16* y, Uint32 color)
{
	int h = y[1] - y[2];
	int l = (x[0] - x[2])/2;
	for (int j = 0; j < h; ++j)
		for (int i = - l * (h - j) / h; i <= l * (h - j) / h; ++i)
		{
			putpixel (window, x[1] + i - 1, y[3] - j, color);
			//putpixel (window, x[1] - i - 1, y[3] - j, color);
			putpixel (window, x[1] + i - 1, y[0] + j, color);
			//putpixel (window, x[1] - i , y[0] + j, color);
		}
}

void Hexagon (SDL_Surface* window, int cx, int cy, int rayon, SDL_Color color)
{
	Sint16 x [6];
	Sint16 y [6];
	for (int i = 0; i < 6; ++i)
	{
		x [i] = cos(i * PI/3 + PI/6) * rayon + cx;
		y [i] = sin(i * PI/3 + PI/6) * rayon + cy;
	}
	//Central_Square (window, x, y, SDL_MapRGB(window->format, color.r, color.g, color.b));
	Triangles (window, x, y, SDL_MapRGB(window->format, color.r, color.g, color.b));
	SDL_Flip (window);
}

void plateau (SDL_Surface* window, int rayon, SDL_Color color)
{
	
	/*int margeh = 20;
	int margev = 20;
	int width = WIDTH - 2 * margeh;
	int height = HEIGHT - 2 * margev;
	int c1, c2;
	if ( (c1 = width / PWIDTH (NBSIDE, 1)) < (c2 = height / PHEIGHT (NBSIDE, 1)))
	{
		int rayon = c1 / NBSIDE;
		printf ("%d", rayon);
		for (int i = 0; i < NBSIDE; ++i)
		{
			int x = margeh + rayon * cos(PI/6) + i * (2 * rayon * cos(PI/6) + 1);
			for (int j = 0; j < NBSIDE; ++j)
			{
				int y = margev + rayon + i * (2 * rayon + 1);
				Hexagon (window, x, y, rayon, color);
			}
		}
	}
	else
	{
		int rayon = c2 / NBSIDE;
		printf ("%d", rayon);
		for (int i = 0; i < NBSIDE; ++i)
		{
			int x = margeh + rayon * cos(PI/6) + i * (2 * rayon * cos(PI/6) + 1);
			for (int j = 0; j < NBSIDE; ++j)
			{
				int y = margev + rayon + i * (2 * rayon + 1);
				Hexagon (window, x, y, rayon, color);
			}
		}
	}*/
}

int main (int argc, char* argv[])
{
	if (SDL_Init (SDL_INIT_VIDEO))
		fprintf (stderr, "Erreur d'inistialisation SDL : %s\n", SDL_GetError());
	
	SDL_Surface* window = SDL_SetVideoMode (WIDTH, HEIGHT, COLOR, SDL_HWSURFACE);
	SDL_FillRect (window, NULL, SDL_MapRGB(window->format, 0, 0, 0));
	
	SDL_Flip (window);
	vector_t ce = {100, 100};
	SDL_Color c = {255, 0, 0};
	int r = 100;
	Hexagon (window, ce.x, ce.y, r, c);
	
	vector_t ce2 = {ce.x + r*cos(PI/6) + 1, ce.y + r + r / 2 + 1};
	
	//Hexagon (window, ce2.x, ce2.y, r, c);
	
	//plateau (window, c);
	/*polygonRGBA(SDL_Surface* window,
                Sint16* vx, Sint16* vy,
                int n,
                Uint8 r, Uint8 g, Uint8 b, Uint8 a);*/
	
	bool end = false;
	
	while (!end)
	{
		SDL_Event event;
		SDL_WaitEvent (&event);
		switch (event.type)
		{
			case SDL_QUIT:
				end = true;
				break;
		}
	}
	SDL_FreeSurface (window);
	SDL_Quit ();
	return 0;
}
