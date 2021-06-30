#include <main_process.h>

int main()
{
	// initialization of SDL graphic library
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("SDL_Init error: %s", SDL_GetError());
		return 1;
	}

	atexit(SDL_Quit);
	//Launch the main process
	t_screen* screen = main_process();
	//then destroy the screen
	SDL_DestroyWindow(screen->screen);
	SDL_DestroyRenderer(screen->renderer);
	free(screen);

	return 0;
}