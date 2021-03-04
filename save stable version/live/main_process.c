#include "main_process.h"

t_screen* main_process()
{
	// creation de the event management system and the infinite loop
	t_screen* screen 		= malloc(sizeof(t_screen));
	screen->screen_number 	= 1;
	t_time* time 			= malloc(sizeof(t_time));
	time->time 				= 0;
       	time->prevTime 		= 0;
	time->deltaTime 		= 0.f;
	screen->nb_mines 		= 0;
	screen->minefield 		= 0;
	screen->score 			=0;
	//SDL_Event event;
	bool isGameRunning = true;
	while (isGameRunning)
	{
		//Exit
		if (screen->screen_number == 0)
			return screen;

		//Title screen
		else if (screen->screen_number == 1)
			Title_screen(screen, time);
		
		//Main game screen
		else if (screen->screen_number == 2)
		{
			Game(screen, time);
		}
		//Win screen
		else if (screen->screen_number == 3)
		{
			Win_screen(screen, time);
		}
		//Death screen
		else if (screen->screen_number == 4)
		{
			Death_screen(screen, time);
		}
	}
	return screen;
}	


	
