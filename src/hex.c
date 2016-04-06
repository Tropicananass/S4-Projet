/********************************************************
 *			Jeux Hex			*
 *	
 ********************************************************/

#include <SDL/SDL.h>
#include "window.h"
#include "globals.h"
#include "affichage_menu_principal.h"
#include "en_jeu.h"

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

int main (void)
{
	SDL_Surface* window = init_window();
	//Affiche_menu_principal(window);
	en_jeu (window);
	SDL_FreeSurface (window);
	SDL_Quit ();
	
	return 0;
}
