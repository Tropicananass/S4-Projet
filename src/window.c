#include "window.h"

#include <SDL/SDL.h>
#include "globals.h"


SDL_Surface* init_window ()
{
	if (SDL_Init (SDL_INIT_VIDEO))
		fprintf (stderr, "Erreur d'inistialisation SDL : %s\n", SDL_GetError());
	
	SDL_Surface* window = SDL_SetVideoMode (DWIDTH, DHEIGHT, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	SDL_FillRect (window, NULL, SDL_MapRGB(window->format, 0, 0, 0));
	SDL_Flip (window);
	return window;
}

SDL_Surface* resize_window (SDL_Surface* window, SDL_Event* event)
{
	int w = event->resize.w, h = event->resize.h;
	while (!SDL_PollEvent(event) || event->type == SDL_VIDEORESIZE)
	{
		w = event->resize.w;
		h = event->resize.h;
	}
	printf ("%d - %d\n", w, h);
	SDL_FreeSurface (window);
	if (w >= DWIDTH && h >= DHEIGHT)
		window = SDL_SetVideoMode(w, h, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	else
		window = SDL_SetVideoMode (DWIDTH, DHEIGHT, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	return window;
}

SDL_Surface* fullscreen_window (SDL_Surface* window)
{
	if (window->flags & SDL_FULLSCREEN)
	{
		window = SDL_SetVideoMode (DWIDTH, DHEIGHT, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	}
	else
	{
		SDL_Rect** modes = SDL_ListModes(window->format, SDL_HWSURFACE  | SDL_DOUBLEBUF| SDL_FULLSCREEN);
		if(modes == (SDL_Rect **)0)
		{
			printf("No modes available!\n");
			exit(-1);
		}

		/* Check if our resolution is restricted */
		if(modes == (SDL_Rect **)-1)
		{
			printf("All resolutions available.\n");
		}
		else{
			/* Print valid modes */
			printf("Available Modes\n");
			for(int i=0;modes[i];++i)
				printf("  %d x %d\n", modes[i]->w, modes[i]->h);
			SDL_FreeSurface (window);
			window = SDL_SetVideoMode(modes[0]->w, modes[0]->h, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
		}
	}
	return window;
}
