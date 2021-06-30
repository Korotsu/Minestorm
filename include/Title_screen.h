#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

// include all the needed librairies
#include "main_process.h"

//create struct needed for the title screen
typedef struct s_pic
{
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rect;
	SDL_Rect src;

} t_pic;

// create the definition of all the title screen function
t_pic* Title_font_init(t_screen* screen);

void Title_print_text(t_screen* screen, t_pic* pic);

#endif
