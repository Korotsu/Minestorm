#include "Title_screen.h"

void Title_screen(t_screen* screen, t_time* time)
{	
	// creation of the title_screen window
	screen->screen = SDL_CreateWindow("Title screen", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_SHOWN);
	screen->renderer = SDL_CreateRenderer(screen->screen, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(screen->renderer, 255, 255, 255, 255);
	t_pic* pic = Title_font_init(screen);
	// creation de the event management system and the infinite loop
	SDL_Event event;
	bool isGameRunning = true;
	while (isGameRunning)
	{
		while (SDL_PollEvent(&event))
		{
			time->time = SDL_GetTicks(); // current time in milliseconds
			if (time->time > time->prevTime)
			{
				time->deltaTime = (time->time - time->prevTime) / 1000.f; // convert deltatime in seconds
				time->prevTime = time->time;
			}
			//print the text
			Title_print_text(screen, pic);

			// allow the player to close the game with the escape keyboard touch
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				isGameRunning = false;
				SDL_DestroyTexture(pic->texture);
				free(pic);
				screen->screen_number = 0;
				return;
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f )
			{
				isGameRunning = false;
				SDL_DestroyTexture(pic->texture);
				free(pic);
				screen->screen_number = 2;
				return;
			}
		}
	}
}

//function to create the text on the screen
t_pic* Title_font_init(t_screen* screen)
{
	t_pic* pic = malloc(sizeof(t_pic));
	//load the picture
	pic->surface = SDL_LoadBMP("../media/Picture/Main_menu.bmp");
	if (pic->surface == NULL)
		return pic;

	SDL_Rect rect = {0,0,1024,768};
	SDL_Rect src = {0,0,1024,768};
	pic->rect = rect;
	pic->src = src;
	pic->texture = SDL_CreateTextureFromSurface(screen->renderer, pic->surface);
	SDL_FreeSurface(pic->surface);
	return pic;
}
void Title_print_text(t_screen* screen, t_pic* pic )
{		
		SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
		SDL_RenderClear(screen->renderer);
		SDL_RenderCopy(screen->renderer, pic->texture, &(pic->src),&(pic->rect));
		SDL_RenderPresent(screen->renderer);
}
