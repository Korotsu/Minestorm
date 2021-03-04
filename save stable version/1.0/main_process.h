#ifndef MAIN_PROCESS_H
#define MAIN_PROCESS_H

// include all the needed librairies
#include <stdlib.h>
#include "SDL2/SDL.h"
#include <stdbool.h>
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "my_put_string.h"
#include <math.h>
#include "my_maths.h"
#include <stdio.h>
#include "alpha_to_number.h"
#include "int_to_alpha.h"
#include "my_maths.h"
#include <float.h>

typedef struct s_screen
{
	unsigned int screen_number;
	SDL_Window* screen;
	SDL_Renderer* renderer;
	unsigned int nb_mines;
} t_screen;

typedef struct s_time
{
	int time;
	int prevTime;
	float deltaTime;
} t_time;

t_screen* main_process();

void Title_screen(t_screen* screen, t_time* time);

void Game(t_screen* screen, t_time* time);

void Win_screen(t_screen* screen, t_time* time);

void Death_screen(t_screen* screen, t_time* time);

#endif
