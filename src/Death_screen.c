#include "Death_screen.h"

void Death_screen(t_screen* screen, t_time* time)
{
	SDL_Event event;
	text_init_d(screen);
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

			// allow the player to close the game with the escape keyboard touch
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				isGameRunning = false;
				screen->screen_number = 0;
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) 
			{
				isGameRunning = false;
				screen->screen_number = 2;
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
			{
				isGameRunning = false;
				screen->screen_number = 1;
			}
		}
	}
}

void text_init_d(t_screen* screen)
{
	SDL_SetWindowTitle(screen->screen, "(-_-)GAME OVER !(-_-)");
	if(TTF_Init() == -1)
	{
		my_put_string("Erreur d'initialisation de TTF_Init : %s\n");
		my_put_string(TTF_GetError());
	}
	TTF_Font* font  = TTF_OpenFont("../media/Fonts/space_invaders.ttf", 300);
	SDL_Color color = { 255, 255, 255, 255 };
	char* word = "Press Return to play again !";
	SDL_Surface* surface = TTF_RenderText_Blended(font, word, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(screen->renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect rect = { 50, 300, 575, 60 };
	word = "Press Space to return to the Main Menu !";
	SDL_Surface* surface2 = TTF_RenderText_Blended(font, word, color);
	SDL_Texture* texture2 = SDL_CreateTextureFromSurface(screen->renderer, surface2);
	SDL_FreeSurface(surface2);
	SDL_Rect rect2 = { 50, 500, 600, 60 };
	
	word = "GAME OVER !";
	SDL_Surface* surface3 = TTF_RenderText_Blended(font, word, color);
	SDL_Texture* texture3 = SDL_CreateTextureFromSurface(screen->renderer, surface3);
	SDL_FreeSurface(surface3);
	SDL_Rect rect3 = { 150, 100, 400, 60 };

	SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
	SDL_RenderClear(screen->renderer);
	SDL_RenderCopy(screen->renderer, texture, NULL, &(rect));
	SDL_RenderCopy(screen->renderer, texture2, NULL, &(rect2));
	SDL_RenderCopy(screen->renderer, texture3, NULL, &(rect3));
	SDL_RenderPresent(screen->renderer);
}
