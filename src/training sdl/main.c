#define _POSIX_C_SOURCE 1

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <SDL_ttf.h>

#define WIDTH 1200
#define HEIGHT 700
#define PI 3.14

#define MIN(a, b) (a < b) ? a : b

//gcc -Wall -std=c99 `sdl-config --cflags --libs` -g -lSDL_image -lSDL_gfx -lm -lSDL_ttf -o main main.c

int main (int argc, char* argv[])
{
	srand(time(NULL));
	if (SDL_Init (SDL_INIT_VIDEO))
		fprintf (stderr, "Erreur d'inistialisation SDL : %s\n", SDL_GetError());
	TTF_Init ();
	SDL_Surface* fenetre = SDL_SetVideoMode (WIDTH, HEIGHT, 32, SDL_HWSURFACE);
	SDL_FillRect (fenetre, NULL, SDL_MapRGB(fenetre->format, 255, 59, 21));
	SDL_Flip (fenetre);
	
	SDL_Rect positionMot = {.x = WIDTH / 2, .y = HEIGHT / 2};
	
	while (1)
	{
		SDL_Event event;
		SDL_WaitEvent (&event);
		switch (event.type)
		{
			case SDL_MOUSEBUTTONUP:
				{
					Uint32 color = SDL_MapRGB (fenetre->format, rand()%255, rand()%255, rand()%255);
					switch (event.button.button)
					{
						case SDL_BUTTON_LEFT:
							SDL_FillRect (fenetre, NULL, color);
							SDL_Flip (fenetre);
							break;
						case SDL_BUTTON_RIGHT:
							{
								int taille[2] = {rand() % WIDTH, rand() % HEIGHT};
								SDL_Surface* carre = SDL_CreateRGBSurface (SDL_HWSURFACE, *taille, *(taille+1) , 32, 0, 0, 0, 0);
								SDL_FillRect (carre, NULL, color);
								SDL_Rect position = {.x = rand() % (WIDTH - *taille), .y = rand() % (HEIGHT - *(taille+1))};
								SDL_BlitSurface (carre, NULL, fenetre, &position);
								SDL_FreeSurface (carre);
								SDL_Flip (fenetre);
								
							}
							break;
						case SDL_BUTTON_MIDDLE:
							{
								SDL_Surface* kiwi = IMG_Load ("kiwi.png");
								int coef = rand ();
								float zoom = MIN((coef % WIDTH) / (float)kiwi->w, (coef % HEIGHT) / (float)kiwi->h);
								kiwi = rotozoomSurface (kiwi, .0, zoom, 1);
								SDL_Rect position = {.x = rand() % (WIDTH - kiwi->w), .y = rand() % (HEIGHT - kiwi->h)};
								SDL_BlitSurface (kiwi, NULL, fenetre, &position);
								SDL_FreeSurface (kiwi);
								SDL_Flip (fenetre);
							}
							break;
					}
				}
				break;
			case SDL_MOUSEMOTION:
				{
					if (event.motion.state == SDL_PRESSED)
					{
						SDL_Color fontColor = {0, 0, 0};
						TTF_Font* penis = TTF_OpenFont ("Dj.ttf", 22);
						SDL_Surface* texte = TTF_RenderText_Solid(penis, "BITE !!!!", fontColor);
						if (abs(positionMot.x - event.motion.x) > texte->w || abs(positionMot.y - event.motion.y) > texte->h)
						{
							positionMot.x = event.motion.x;
							positionMot.y = event.motion.y;
							SDL_BlitSurface (texte, NULL, fenetre, &positionMot);
							SDL_FreeSurface (texte);
							SDL_Flip (fenetre);
						}
					}
					else
					{
						SDL_Surface* troll = IMG_Load ("troll.png");
						troll = rotozoomSurface (troll, .0, .2, 1);
						if (abs(positionMot.x - event.motion.x + troll->w /2) > troll->w || abs(positionMot.y - event.motion.y + troll->h /2) > troll->h)
						{
							positionMot.x = MIN(WIDTH - troll->w, event.motion.x - troll->w /2);
							positionMot.y = MIN(HEIGHT - troll->h, event.motion.y - troll->h /2);
							SDL_BlitSurface (troll, NULL, fenetre, &positionMot);
							SDL_FreeSurface (troll);
							SDL_Flip (fenetre);
						}
					}
				}
		}
		
		/*if (event.type == SDL_MOUSEBUTTONUP && SDL_BUTTON_LEFT == event.button.button)
		{
			SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, rand()%255, rand()%255, rand()%255));
			SDL_Flip(fenetre);
		}*/
		if (event.type == SDL_QUIT)
			break;
	}
	
	SDL_FreeSurface (fenetre);
	TTF_Quit ();
	SDL_Quit ();
	return 0;
}
