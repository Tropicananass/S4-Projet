#include "window.h"

int event_menu_principal (SDL_Event* event)
{
	while (1)
	{
		SDL_WaitEvent (event);
		switch (event->type)
		{
			case SDL_MOUSEMOTION:
				return 1;
				break;
			case SDL_BUTTON_LEFT:
				break;
		}
	}
	return -1;
}
