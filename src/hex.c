/********************************************************
 *			Jeux Hex			*
 *	
 ********************************************************/

#include <SDL/SDL.h>
#include "window.h"
#include "globals.h"
#include "menu_principal.h"
#include "en_jeu.h"
#include "param.h"

#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <time.h>   // for nanosleep
#else
#include <unistd.h> // for usleep
#endif

void sleep_ms(int milliseconds) // cross-platform sleep function
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}

int main(int argc, char **argv)
{
	SDL_Surface* window = init_window();
	load_param(window);
	Background (window);
	bool end = 0;
	while (!end)
	{
		int retour = menu_principal(window);
		reset_window(window);
		switch (retour)
		{
		case M_JOUER:
			en_jeu (window);
			reset_window(window);
			break;
		case M_QUITTER:
			end = 1;
		}
	}
	save_param(window);
	SDL_FreeSurface (window);
	SDL_Quit ();
	return 0;
}
