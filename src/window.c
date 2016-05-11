#include "window.h"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "globals.h"
#include "param.h"


SDL_Surface* init_window ()
{
	if (SDL_Init (SDL_INIT_VIDEO))
	{
		fprintf (stderr, "SDL_Init failed : %s\n", SDL_GetError());
		exit (1);
	}
	if (TTF_Init ())
	{
		fprintf (stderr, "TTF_Init failed : %s\n", SDL_GetError());
		exit (2);
	}

	SDL_WM_SetIcon (SDL_LoadBMP("ressources/ico.bmp"), NULL);
	SDL_WM_SetCaption ("HEX (...a saute !)", "");

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		fprintf(stderr, "Mix init failed : %s\n", Mix_GetError());
	Mix_AllocateChannels(3); //MIX_CHANNELS

	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	int bpp;
	if (info == NULL)
	{
		fprintf (stderr, "Unable to get video information\n Trying to force BPP to 8.\n");
		bpp = 8;
	}
	else
		bpp = info->vfmt->BitsPerPixel;

	SDL_Surface* window = SDL_SetVideoMode (DWIDTH, DHEIGHT, bpp, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	if (window == NULL)
	{
		fprintf (stderr, "Unable to set video mode : %s\n", SDL_GetError());
		exit (3);
	}
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
	SDL_FreeSurface (window);
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	int bpp;
	if (info == NULL)
	{
		fprintf (stderr, "Unable to get video information\n Trying to force BPP to 8.\n");
		bpp = 8;
	}
	else
		bpp = info->vfmt->BitsPerPixel;
	if (w >= DWIDTH && h >= DHEIGHT)
		window = SDL_SetVideoMode(w, h, bpp, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	else
		window = SDL_SetVideoMode (DWIDTH, DHEIGHT, bpp, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	if (window == NULL)
	{
		fprintf (stderr, "Unable to set video mode : %s\n", SDL_GetError());
		exit (3);
	}
	SDL_FillRect (window, NULL, param->background);
	SDL_Flip (window);
	return window;
}

SDL_Surface* fullscreen_window (SDL_Surface* window)
{
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	int bpp;
	if (info == NULL)
	{
		fprintf (stderr, "Unable to get video information\n Trying to force BPP to 8.\n");
		bpp = 8;
	}
	else
		bpp = info->vfmt->BitsPerPixel;

	if (window->flags & SDL_FULLSCREEN)
	{
		window = SDL_SetVideoMode (DWIDTH, DHEIGHT, bpp, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	}
	else
	{
		SDL_Rect** modes = SDL_ListModes(window->format, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
		/*if(modes == (SDL_Rect **)0)
		{
			printf("No modes available!\n");
		}*/

		/* Check if our resolution is restricted */
		/*if(modes == (SDL_Rect **)-1)
		{
			printf("All resolutions available.\n");
		}
		else{*/
		if(modes != (SDL_Rect **)-1)
		{
			/* Print valid modes
			printf("Available Modes\n");
			for(int i=0;modes[i];++i)
				printf("  %d x %d\n", modes[i]->w, modes[i]->h);*/
			SDL_FreeSurface (window);
			window = SDL_SetVideoMode(modes[0]->w, modes[0]->h, bpp, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
		}
	}
	if (window == NULL)
	{
		fprintf (stderr, "Unable to set video mode : %s\n", SDL_GetError());
		exit (3);
	}
	SDL_FillRect (window, NULL, param->background);
	SDL_Flip (window);
	return window;
}

void Reset_window (SDL_Surface* window)
{
	SDL_FillRect (window, NULL, param->background);
	SDL_Flip (window);
}

void Free_window (SDL_Surface* window)
{
	SDL_FreeSurface (window);
	SDL_Quit();
	TTF_Quit();
	Mix_CloseAudio();
}
