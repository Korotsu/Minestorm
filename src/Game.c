#include <assert.h>
#include "Game.h"

void* first 	= NULL;
void* second 	= NULL;
void* third     = NULL;
void* fourth  	= NULL;

void Game(t_screen* screen, t_time* time)
{	
	t_object* object 		= Game_init(screen);
	object->score.score_int = screen->score;
	time->spawn_time_m 		= SDL_GetTicks();
	bool is_mines_init 		= false;
	bool is_game_paused 	= false;
	SDL_Event event;
	bool isGameRunning = true;
	while (isGameRunning)
	{
		if (SDL_PollEvent(&event) == 1)
		{
			// allow the player to close the game with the escape keyboard touch
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				isGameRunning = false;
				Game_destroy(object,time,screen);
				screen->screen_number = 0;
			}
		}
		time->time = SDL_GetTicks(); // current time in milliseconds 

		if (time->time > time->prevTime)
		{
			time->deltaTime = (time->time - time->prevTime) / 1000.f; // convert deltatime in seconds
			time->prevTime = time->time;
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
		{
			is_game_paused = Is_game_paused(is_game_paused, time, screen,object);
		}

		if (!is_game_paused)
		{
			if ( !is_mines_init && (time->time - time->spawn_time_m)/1000 >= 5)
			{ 
				create_mine_list(object);
				is_mines_init = true;
			}

			Player_movement(object, event, time);
			Player_fire_teleport(object,time);

			if (object->player.is_alive == 0)
			{
				isGameRunning = false;
				Game_destroy(object,time,screen);
				screen->screen_number = 2;
			}

			if (screen->win == true)
			{
				isGameRunning = false;
				Game_destroy(object,time,screen);
				screen->screen_number = 2;
				screen->minefield++;
			}
			
			if (isGameRunning)
				Game_print_text(screen, object,time);
		}
	}


}

void text_pause(t_screen* screen)
{
	SDL_Color color = { 255, 255, 255, 255 };
	char* word = "Pause";
	SDL_Surface* surface = TTF_RenderText_Blended(screen->font, word, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(screen->renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect rect = { 334, 250, 356, 100};

	SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
	SDL_RenderCopy(screen->renderer, texture, NULL, &(rect));
	SDL_RenderPresent(screen->renderer);
}

bool Is_game_paused(bool is_game_paused, t_time* time, t_screen* screen, t_object* object)
{
	time->time_paused = SDL_GetTicks();

	if (is_game_paused == true)
	{
		return false;
	}

	else if (is_game_paused == false)
	{
		text_pause(screen);
		return true;
	}

	return is_game_paused;
}

void init_screen(t_screen* screen)
{
	//clear the screen
	SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
	SDL_RenderClear(screen->renderer);
	SDL_SetWindowTitle(screen->screen, "((+_+))((+_+))MineStorm((+_+))((+_+))");
	if(TTF_Init() == -1)
	{
		my_put_string("Erreur d'initialisation de TTF_Init : %s\n");
		my_put_string(TTF_GetError());
	}
	screen->font  = TTF_OpenFont("../media/Fonts/space_invaders.ttf", 300);
}

t_pic Map_init(t_screen* screen, t_object* object)
{
	t_pic pic 		= object->pic;
	pic.surface 	= SDL_LoadBMP("../media/Picture/star.bmp");
	if (pic.surface == NULL)
		return pic;

	SDL_Rect rect 	= {0,0,1024,768};
	SDL_Rect src 	= {0,0,1024,768};
	pic.rect 		= rect;
	pic.src 		= src;
	pic.texture 	= SDL_CreateTextureFromSurface(screen->renderer, pic.surface);
	SDL_FreeSurface(pic.surface);
	return pic;
}

void player_init(t_object* object)
{
	object->player.is_alive 				= 1;
	object->player.speed 					= 0.0;
	object->player.origin.x					= 512;
	object->player.origin.y					= 384;
	object->player.I.x 						= 1;
	object->player.I.y 						= 0;
	object->player.J.x 						= 0;
	object->player.J.y 						= -1;
	object->player.polygon[0].l_list[0].x 	= 0;
	object->player.polygon[0].l_list[0].y 	= 0;
	object->player.polygon[0].l_list[1].x 	= -10;
	object->player.polygon[0].l_list[1].y 	= 10;
	object->player.polygon[0].l_list[2].x 	= 0;
	object->player.polygon[0].l_list[2].y 	= -10;
	object->player.polygon[0].l_list[3].x 	= 0;
	object->player.polygon[0].l_list[3].y 	= 0;
	object->player.polygon[1].l_list[0].x 	= 0;
	object->player.polygon[1].l_list[0].y 	= 0;
	object->player.polygon[1].l_list[1].x 	= 10;
	object->player.polygon[1].l_list[1].y 	= 10;
	object->player.polygon[1].l_list[2].x 	= 0;
	object->player.polygon[1].l_list[2].y 	= -10;
	object->player.polygon[1].l_list[3].x 	= 0;
	object->player.polygon[1].l_list[3].y 	= 0;
	object->player.Velocity.x 				= 0.0;
	object->player.Velocity.y 				= 0.0;
	
}

void create_mine(t_object* object, unsigned int size, unsigned int type_m, unsigned int index)
{	
	t_mine* mine 						= object->mine;
	mine[index].size 					= size;
	mine[index].type_m					= type_m;
	mine[index].speed 					= 4 - mine->size;
	int i 								= object->Spawn_list.size - 1;
	mine[index].origin.x				= object->Spawn_list.list[i].x;
	mine[index].origin.y				= object->Spawn_list.list[i].y;
	mine[index].nb_polygon 				= 4;
	mine[index].is_alive 				= true;

	if (type_m == 2)
	{
		mine[index].score_value	+= 325;	
	}
	
	else if (type_m == 3)
	{
		mine[index].score_value	+= 500; 	
	}

	else if (type_m == 4)
	{
		mine[index].score_value	+= 750; 	
	}

	if ( type_m == 1)
	{
		mine[index].nb_polygon = 3;
		mine[index].score_value	+= 100;
	}

	if (size == 1)
	{
		mine[index].score_value	+= 100;
	}

	else if (size == 2)
	{
		mine[index].score_value	+= 35;
	}
	
	mine[index].polygon = calloc(mine[index].nb_polygon, sizeof(t_polygon));
	float coord_list_0[] = {0,0,
							0,-4,
							-10,-10,
							-2.5,0,
							0,0,-2.5,0/**/,0,10/**/,2.5,0/**/,0,0/**/,2.5,0/**/,10,-10/**/,0,-4/**/,0,0/**/};
	float coord_list_1[] = {0,0/**/,0,5.0/**/,-10,10/**/,-5.0,0/**/,0,0/**/,-5.0,0/**/,-10,-10/**/,0,-5.0,/**/0,0/**/,0,-5.0/**/,10,-10,/**/5.0,0,/**/0,0,/**/5.0,0,/**/10,10,/**/0,5.0,/**/0,0/**/};
	float coord_list_2[] = {0,0/**/,0,2.5/**/,-10,10/**/,-2.5,0/**/,0,0/**/,-2.5,0/**/,-10,-10/**/,0,-2.5,/**/0,0/**/,0,-2.5/**/,10,-10,/**/2.5,0,/**/0,0,/**/2.5,0,/**/10,10,/**/0,2.5,/**/0,0/**/};
	float coord_list_3[] = {0,0,/**/0,-7.5,-10,0,0,0,-7.5,0/**/,0,10/**/,0,0/**/,0,7.5/**/,10,0/**/,0,0/**/,7.5,0/**/,0,-10/**/,0,0/**/};

	float* coords_by_type[4] = {
		coord_list_0,
		coord_list_1,
		coord_list_2,
		coord_list_3,
	};
	int coords_size_by_type[4] = {
		sizeof(coord_list_0) / sizeof(coord_list_0[0]),
		sizeof(coord_list_1) / sizeof(coord_list_1[0]),
		sizeof(coord_list_2) / sizeof(coord_list_2[0]),
		sizeof(coord_list_3) / sizeof(coord_list_3[0]),
	};

	float* coords = coords_by_type[type_m-1];
	int coords_size = coords_size_by_type[type_m-1];
	unsigned int k = 0;
	for ( unsigned int i = 0 ; i < mine[index].nb_polygon ; i++)
	{
		if ( i == 0 )
		{
			mine[index].polygon[i].size = coords_size / (2 * mine[index].nb_polygon) +1;
			mine[index].polygon[i].l_list = calloc(mine[index].polygon[i].size, sizeof(SDL_Point));
			mine[index].polygon[i].w_list = calloc(mine[index].polygon[i].size, sizeof(SDL_Point));
		}

		else
		{
			mine[index].polygon[i].size = coords_size / (2 * mine[index].nb_polygon) ;
			mine[index].polygon[i].l_list = calloc(mine[index].polygon[i].size, sizeof(SDL_Point));
			mine[index].polygon[i].w_list = calloc(mine[index].polygon[i].size, sizeof(SDL_Point));
		}
		
		for ( unsigned int j = 0 ; j < mine[index].polygon[i].size ; j++)
		{
			mine[index].polygon[i].l_list[j].x = (coords[k]) * mine[index].size ;
			mine[index].polygon[i].l_list[j].y = (coords[k + 1]) * mine[index].size;
			k += 2; //two by two.
		}
	}

	object->Spawn_list.size--;
	mine[index].I.x 		= 1;
	mine[index].I.y 		= 0;
	mine[index].J.x 		= 0;
	mine[index].J.y 		= -1;
	mine[index].I = rotate_vector( mine[index].I, (rand() % (360 - 0 + 1)) + 0);
	mine[index].J 	= normal_rotate_90_vector(mine[index].I);
}

void control_panel(Control_Panel* panel)
{
	panel->number_of_S3_T1 = panel->Control_list[0];
	panel->number_of_S3_T2 = panel->Control_list[1];
	panel->number_of_S3_T3 = panel->Control_list[2];
	panel->number_of_S3_T4 = panel->Control_list[3];
	panel->number_of_Minelayer = 0;
	free(panel->Control_list);

// Attention the number of mines do not have to overpass the numbers of mines and should be a multiple of 7.
// Do not touch the Fireball and Minelayer numbers it may crash the game because its self-controled by the game.
}

void create_mine_list(t_object* object)
{
	Control_Panel Control_Panel = object->Control_Panel;
	unsigned int index 			= 0;
	unsigned int index_nb 		= 0;

	while ( index < Control_Panel.number_of_S3_T1 + index_nb )
	{
		create_mine(object, 3, 1, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S3_T2 + index_nb )
	{
		create_mine(object, 3, 2, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S3_T3 + index_nb )
	{
		create_mine(object, 3, 3, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S3_T4 + index_nb )
	{
		create_mine(object, 3, 4, index);
		index++;
	}
}

t_object* Game_init(t_screen* screen)
{
	init_screen(screen);
	t_object* object 		= calloc(1, sizeof(t_object));
	generate_minefield(screen,object);
	screen->win 			= false;
	object->nb_mines 		= screen->nb_mines;
	object->mine 			= calloc(screen->nb_mines+1, sizeof(t_mine));
	object->player.polygon 	= calloc(2, sizeof(t_polygon));

	for ( unsigned int i = 0 ; i < 2 ; i++)
	{
		object->player.polygon[i].size = 4;
		object->player.polygon[i].w_list = calloc(object->player.polygon[i].size, sizeof(SDL_Point));
		object->player.polygon[i].l_list = calloc(object->player.polygon[i].size, sizeof(SDL_Point));
	}

	t_pic pic = Map_init(screen, object);
	
	player_init(object);
	control_panel(&object->Control_Panel);
	Generate_random_spawn_pts(object, screen->nb_mines);
	object->pic = pic;

	return object;
}

void generate_minefield(t_screen* screen, t_object* object)
{
	//Control_list = {nb_t1_s3,nb_t2_s3,nb_t3_s3,nb_t4_s3};
	object->Control_Panel.Control_list = calloc(4, sizeof(int));
	object->Control_Panel.Control_list[0] =  1; 
	int temp = screen->minefield;
	int temp_x;

	while ( temp != 0 )
	{
		if ( temp - 4 >= 0 )
		{
			temp_x = object->Control_Panel.Control_list[3];
			object->Control_Panel.Control_list[3] = temp_x + 1 ;
			temp -= 4;
		}

		else if ( temp - 3 >= 0 )
		{
			temp_x = object->Control_Panel.Control_list[2];
			object->Control_Panel.Control_list[2] = temp_x + 1 ;
			temp -= 3;
		}

		else if ( temp - 2 >= 0 )
		{
			temp_x = object->Control_Panel.Control_list[1];
			object->Control_Panel.Control_list[1] = temp_x + 1 ;
			temp -= 2;
		}

		else if ( temp - 1 == 0 )
		{
			temp_x = object->Control_Panel.Control_list[0];
			object->Control_Panel.Control_list[0] = temp_x + 1 ;
			temp -= 1;
		}
		
	}
	screen->nb_mines = object->Control_Panel.Control_list[0] * 7 +
						object->Control_Panel.Control_list[1] * 7 +
						object->Control_Panel.Control_list[2] * 7 +
						object->Control_Panel.Control_list[3] * 7;
	
}

void destroy_player(t_object* object)
{
	object->player.origin.x -= FLT_MAX;
	object->player.I.x = 0.0;
	object->player.I.y = 0.0;
	object->player.J.x = 0.0;
	object->player.J.y = 0.0;
	object->player.is_alive = 0;
}
void print_player(t_object* object, t_screen* screen)
{
	t_player player = object->player;
	for ( unsigned int index = 0 ; index < 2 ; index++)
		SDL_RenderDrawLines(screen->renderer, LocalToWorld_list(player.polygon[index].l_list,
															player.polygon[index].w_list,
															player.origin, 
															player.I, 
															player.J, 
															player.polygon[index].size), 
															player.polygon[index].size);
	
}

void print_score(t_object* object, t_screen* screen)
{
	object->score.score_text = int_to_alpha(object->score.score_int);
	SDL_Color color1 = { 255, 255, 255, 255};
	object->score.surface = TTF_RenderText_Blended(screen->font, object->score.score_text, color1);
	object->score.texture = SDL_CreateTextureFromSurface(screen->renderer, object->score.surface);
	SDL_FreeSurface(object->score.surface);
	SDL_Rect rect_s = {150 , 0, 50 * string_length(object->score.score_text), 40};
	object->score.rect = rect_s;
	char* word = "SCORE";
	SDL_Surface* text_surface = TTF_RenderText_Blended(screen->font, word, color1);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(screen->renderer, text_surface);
	SDL_FreeSurface(text_surface);
	SDL_Rect rect = { 0, 0, 130, 40 };
	SDL_RenderCopy(screen->renderer,text_texture,NULL,&rect);
	SDL_RenderCopy(screen->renderer,object->score.texture, NULL, &rect_s);
	SDL_DestroyTexture(text_texture);
	SDL_DestroyTexture(object->score.texture);

}

void print_mines(t_object* object, t_screen* screen)
{
	t_mine* mine = object->mine;
	for ( unsigned int index = 0 ; index < screen->nb_mines ; index++ )
	{
		//mine[index].origin = mine_movement(mine[index]);
		for (unsigned int i = 0; i < mine[index].nb_polygon ; i++)
		{
			mine[index].polygon[i].w_list = LocalToWorld_list(mine[index].polygon[i].l_list,
								mine[index].polygon[i].w_list,
								mine[index].origin, 
								mine[index].I, 
								mine[index].J, 
								mine[index].polygon[i].size);
		}
		
		if (mine[index].is_alive)
		{
			mine[index].origin = mine_movement(mine[index],object);
			for (unsigned int i = 0; i < mine[index].nb_polygon ; i++)
			{
				SDL_RenderDrawLines(screen->renderer, mine[index].polygon[i].w_list, mine[index].polygon[i].size);
			}
		}

		if (is_player_mine_collide(object,index)) 
		{
			destroy_player(object);
			mine[index].is_alive = 0;
			mine[index].origin.x -= FLT_MAX;
			mine[index].speed = 0;
		}
		
	}
}

void Generate_random_spawn_pts(t_object* object, unsigned int nb_mines)
{
	object->Spawn_list.size 		= nb_mines;
	object->Spawn_list.list 		= calloc(object->Spawn_list.size, sizeof(SDL_Point));
	object->Spawn_list.origin_l 	= calloc(object->Spawn_list.size, sizeof(Point));
	object->Spawn_list.list_size    = 9;
	object->Spawn_list.l_list 		= calloc(object->Spawn_list.list_size  , sizeof(SDL_Point));
	object->Spawn_list.w_list 		= calloc(object->Spawn_list.list_size  , sizeof(SDL_Point));
	object->Spawn_list.I			= object->player.I;
	object->Spawn_list.J			= object->player.J;
	object->Spawn_list.radius		= 5;
	unsigned int k 					= 0;
	float coords_list[]				= {-object->Spawn_list.radius,0,
										-object->Spawn_list.radius*cos(45),object->Spawn_list.radius*sin(45),
										0,object->Spawn_list.radius,
										object->Spawn_list.radius*cos(45),object->Spawn_list.radius*sin(45),
										object->Spawn_list.radius,0,
										object->Spawn_list.radius*cos(45),-object->Spawn_list.radius*sin(45),
										0,object->Spawn_list.radius,
										-object->Spawn_list.radius*cos(45),-object->Spawn_list.radius*sin(45),
										-object->Spawn_list.radius,0};

	for (unsigned int i = 0; i < object->Spawn_list.list_size ; i++)
	{
		object->Spawn_list.l_list[i].x = coords_list[k];
		object->Spawn_list.l_list[i].y = coords_list[k+1];
		k += 2;
	}

	for ( unsigned int index = 0 ; index < nb_mines ; index++)
	{
		object->Spawn_list.list[index].x 		= (rand() % (1024 - 5 + 1)) + 5;
		object->Spawn_list.list[index].y 		= (rand() % (768 - 5 + 1)) + 5;
		object->Spawn_list.origin_l[index].x 	= object->Spawn_list.list[index].x;
		object->Spawn_list.origin_l[index].y 	= object->Spawn_list.list[index].y;
	}
}

void print_spawn_points(t_screen* screen,t_object* object)
{
	for (unsigned int index = 0; index < object->Spawn_list.size ; index++)
	{
		object->Spawn_list.w_list = LocalToWorld_list(object->Spawn_list.l_list,
															object->Spawn_list.w_list,
															object->Spawn_list.origin_l[index], 
															object->Spawn_list.I, 
															object->Spawn_list.J, 
															object->Spawn_list.list_size);
		SDL_RenderDrawLines(screen->renderer, object->Spawn_list.w_list, object->Spawn_list.list_size);
	}
}

void Game_print_text(t_screen* screen, t_object* object,t_time* time)
{
	SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
	SDL_RenderClear(screen->renderer);
	SDL_RenderCopy(screen->renderer, object->pic.texture, &(object->pic.src),&(object->pic.rect));
	print_score(object,screen);
	SDL_SetRenderDrawColor(screen->renderer, 63, 191, 63, 0);
	print_spawn_points(screen,object);
	SDL_SetRenderDrawColor(screen->renderer, 33, 237, 237, 255);
	print_player(object, screen);
	Player_fire_process(object,screen,time);
	SDL_SetRenderDrawColor(screen->renderer, 255, 0, 0, 0);
	if ((time->time - time->spawn_time_m)/1000 >= 5 )
	{
		SDL_SetRenderDrawColor(screen->renderer, 255, 0, 0, 0);
		print_mines(object,screen);
	}
	SDL_RenderPresent(screen->renderer);
}

void Player_movement(t_object* object, SDL_Event event, t_time* time)
{
	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	if (keystates[SDL_SCANCODE_D])
	{
		object->player.I = rotate_vector(object->player.I, 355);
		object->player.J = rotate_vector(object->player.I, 90);	
	}

	else if (keystates[SDL_SCANCODE_G])
	{
		object->player.I = rotate_vector(object->player.I, 5);
		object->player.J = rotate_vector(object->player.I, 90);	
	}

	
	if (keystates[SDL_SCANCODE_R])
	{
		object->player.speed += 50;
		object->player.Acceleration.x = object->player.speed * object->player.J.x * time->deltaTime;
		object->player.Acceleration.y = object->player.speed * object->player.J.y * time->deltaTime;
		object->player.Velocity.x += object->player.Acceleration.x * time->deltaTime;
		object->player.Velocity.y += object->player.Acceleration.y * time->deltaTime;
	}

	if (object->player.speed > 0)
	{
		object->player.speed -= 10;
	}

	if (object->player.speed > 1000.0 )
	{
		object->player.speed = 1000.0;
	}
	
	if (keystates[SDL_SCANCODE_R] && vector_length(object->player.Velocity) <= 10)
	{
		object->player.origin.x -= object->player.Velocity.x; 
		object->player.origin.y -= object->player.Velocity.y;
	}

	else
	{
		object->player.origin.x -= object->player.Velocity.x; 
		object->player.origin.y -= object->player.Velocity.y;
		object->player.Velocity.x -= (object->player.Velocity.x/30);
		object->player.Velocity.y -= (object->player.Velocity.y/30);
	}

	if (object->player.origin.x >= 1023)
	{
		object->player.origin.x = 0.1;
	}
	if (object->player.origin.x <= 0)
	{
		object->player.origin.x = 1024;
	}

	if (object->player.origin.y >= 767)
	{
		object->player.origin.y = 0.1;
	}

	if (object->player.origin.y <= 0)
	{
		object->player.origin.y = 768;
	}
}

SDL_Point LocalToWorld_pts(SDL_Point pts, Point origin , Vector2 I, Vector2 J) 
{
	return (SDL_Point) { origin.x + ( ( pts.x * I.x ) + ( pts.y * J.x )),
					 	origin.y + ( ( pts.x * I.y ) + ( pts.y * J.y ))};
}

SDL_Point* LocalToWorld_list(SDL_Point* list , SDL_Point* w_list, Point origin, Vector2 I, Vector2 J, unsigned int size)
{
	for ( unsigned int index = 0 ; index < size ; index++)
	{
		w_list[index] = LocalToWorld_pts(list[index], origin, I, J);
	}
	return w_list;
}

SDL_Point WorldToLocal_pts(SDL_Point pts, Point origin, Vector2 I, Vector2 J)
{
	return (SDL_Point) { 
		( pts.x - origin.x ) * I.x + (pts.y - origin.y) * I.y,
		( pts.x - origin.x ) * J.x + (pts.y - origin.y) * J.y};
}

SDL_Point* WorldToLocal_list(SDL_Point* list, Point origin, Vector2 I, Vector2 J, unsigned int size)
{
	SDL_Point* l_list = calloc(size, sizeof(SDL_Point));
	for ( unsigned int index = 0 ; index < size ; index++)
	{
		l_list[index] = WorldToLocal_pts(list[index], origin, I, J);
	}
	return l_list;
}

Vector2 LocalToWorld_vector(Vector2 vector, t_object* object)
{
	return (Vector2) { ( ( vector.x * object->player.I.x ) + ( vector.y * object->player.J.x )),
					 ( ( vector.x * object->player.I.y ) + ( vector.y * object->player.J.y ))};
}

Vector2 WorldToLocal_vector(Vector2 vector, t_object* object)
{
	return (Point) { ( ( vector.x * object->player.I.x ) + ( vector.y * object->player.J.x )) * object->player.I.x,
					 ( ( vector.x * object->player.I.y ) + ( vector.y * object->player.J.y )) * object->player.J.y};
}

bool is_player_mine_collide(t_object* object, unsigned int index_m)
{
	for ( unsigned int index_p = 0; index_p < object->mine[index_m].nb_polygon ; index_p++ )
	{
		for ( unsigned int i2 = 0 ; i2 < 2 ; i2++ )
		{
			
			if ( is_polygon_collide(object, index_m, index_p, i2))
			{
				return true;
			}
		}	
	}
	
	return false;
}

bool is_mine_bullet_collide(t_object* object, t_bullet* bullet, unsigned int index_m)
{
	for ( unsigned int index_p = 0; index_p < object->mine[index_m].nb_polygon ; index_p++ )
	{
		if (is_polygon_bullet_mine_collide(object,index_m,index_p,bullet))
		{
			return true;
		}
	}
	return false;
}

bool is_polygon_bullet_mine_collide(t_object* object, unsigned int index_m, unsigned int index_p, t_bullet* bullet)
{
	for (unsigned int i = 0; i < object->mine[index_m].polygon[index_p].size ; i++)
	{
		bool mineToBullet = is_range_collide(object->mine[index_m].polygon[index_p],
											bullet->polygon,
											object->mine[index_m].polygon[index_p].w_list[i],
											object->mine[index_m].polygon[index_p].w_list[i+1]);
		if ( !mineToBullet )
		{
			return false;
		}

		if ( i < bullet->polygon.size)
		{
			bool bulletToMine = is_range_collide(bullet->polygon,
												object->mine[index_m].polygon[index_p],
												bullet->polygon.w_list[i],
												bullet->polygon.w_list[i+1]);
			if ( !bulletToMine )
			{
				return false;
			}
		}
	}
	return true;
}

bool is_player_bullet_collide(t_object* object, t_bullet* bullet)
{
	for ( unsigned int i2 = 0; i2 < 2; i2++)
	{
		if (is_polygon_bullet_player_collide(object,i2,bullet))
		{
			return true;
		}
		
	}
	return false;
}

bool is_player_fireball_collide(t_object* object, Fireball* bullet)
{
	for ( unsigned int i2 = 0; i2 < 2; i2++)
	{
		if (is_polygon_fireball_player_collide(object,i2,bullet))
		{
			return true;
		}
		
	}
	return false;
}

bool is_bullet_fireball_collide(t_object* object, t_bullet* bullet, Fireball* fireball)
{
	float distance_between_them = sqrt((bullet->origin.x - fireball->origin.x) * (bullet->origin.x - fireball->origin.x) + 
    (fireball->origin.y - bullet->origin.y) * (fireball->origin.y - bullet->origin.y));

	if (distance_between_them <= bullet->radius + fireball->radius)
	{
		return true;
	}

	return false;
}

bool is_polygon_bullet_player_collide(t_object* object, unsigned int i2, t_bullet* bullet)
{
	for (unsigned int i = 0; i < object->player.polygon[i2].size ; i++)
	{
		bool playerToBullet = is_range_collide(object->player.polygon[i2],
											bullet->polygon,
											object->player.polygon[i2].w_list[i],
											object->player.polygon[i2].w_list[i+1]);
		if ( !playerToBullet )
		{
			return false;
		}

		if ( i < bullet->polygon.size)
		{
			bool bulletToPlayer = is_range_collide(bullet->polygon,
												object->player.polygon[i2],
												bullet->polygon.w_list[i],
												bullet->polygon.w_list[i+1]);
			if ( !bulletToPlayer )
			{
				return false;
			}
		}
	}
	return true;
}

bool is_polygon_fireball_player_collide(t_object* object, unsigned int i2, Fireball* bullet)
{
	for (unsigned int i = 0; i < object->player.polygon[i2].size ; i++)
	{
		bool playerToBullet = is_range_collide(object->player.polygon[i2],
											bullet->polygon,
											object->player.polygon[i2].w_list[i],
											object->player.polygon[i2].w_list[i+1]);
		if ( !playerToBullet )
		{
			return false;
		}

		if ( i < bullet->polygon.size)
		{
			bool bulletToPlayer = is_range_collide(bullet->polygon,
												object->player.polygon[i2],
												bullet->polygon.w_list[i],
												bullet->polygon.w_list[i+1]);
			if ( !bulletToPlayer )
			{
				return false;
			}
		}
	}
	return true;
}

bool is_polygon_collide(t_object* object, unsigned int index_m, unsigned int index_p , unsigned int i2)
{
	for ( unsigned int i = 0 ; i < object->mine[index_m].polygon[index_p].size ; i++ )
	{
		bool mineToPlayer = is_range_collide( object->mine[index_m].polygon[index_p],
								object->player.polygon[i2],
								object->mine[index_m].polygon[index_p].w_list[i],
								object->mine[index_m].polygon[index_p].w_list[i+1]);

		if ( !mineToPlayer )
		{
			return false;
		}

		if ( i < object->player.polygon[i2].size)
		{
			bool playerToMine = is_range_collide( object->player.polygon[i2],
								object->mine[index_m].polygon[index_p],
								object->player.polygon[i2].w_list[i],
								object->player.polygon[i2].w_list[i+1]);								
			if ( !playerToMine )
			{
				return false;
			}
		}
	}

	return true;
}
bool is_range_collide(t_polygon polygon , t_polygon polygon2, SDL_Point a, SDL_Point b)
{
	Vector2 AB;
	AB.x 		=  b.x - a.x;
	AB.y 		= b.y - a.y;
	Vector2 n 	= normal_rotate_90_vector(AB);
	n 			= Unit_vector_computing(n);
	float* min_m 	= malloc(sizeof(float));
	float* max_m 	= malloc(sizeof(float));
	float* min_p 	= malloc(sizeof(float));
	float* max_p 	= malloc(sizeof(float));

	range_computing(n, polygon.w_list, polygon.size, min_m, max_m);
	range_computing(n, polygon2.w_list, polygon2.size, min_p, max_p);
	if ( max_m[0] < min_p[0] )
	{
		free(min_m);
		free(max_m);
		free(min_p);
		free(max_p);
		return false;
	}

	if ( min_m[0] > max_p[0] ) 
	{
		free(min_m);
		free(max_m);
		free(min_p);
		free(max_p);
		return false;
	}

	free(min_m);
	free(max_m);
	free(min_p);
	free(max_p);
	return true;
}

void range_computing(Vector2 n, SDL_Point* list, unsigned int len, float* min, float* max)
{
	Vector2* v_list = malloc(len * sizeof(Vector2));

	for ( unsigned int index = 0 ; index < len ; index++ )
	{
		v_list[index].x = list[index].x;
		v_list[index].y = list[index].y;
	}

	float* r_list = malloc(len * sizeof(float));
	float a;
	for (unsigned int  i = 0; i < len ; i++)
	{
		a = dot_product(v_list[i], n);
		r_list[i] = a;
	}
	
	float min_range = FLT_MAX;
	float max_range = -FLT_MAX;

	for (unsigned int i = 0; i < len; i++)
	{
		if ( min_range > r_list[i])
		{
			min_range = r_list[i];
		}

		if ( max_range < r_list[i])
		{
			max_range = r_list[i];
		}
	}
	min[0] = min_range;
	max[0] = max_range;
	free(v_list);
	free(r_list);
}

void Player_fire_teleport(t_object* object, t_time* time)
{
	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	if ((keystates[SDL_SCANCODE_F] || keystates[SDL_SCANCODE_KP_5]) && (time->time - time->fire_time)/1000 >= 0.15)
	{
		time->fire_time = SDL_GetTicks();
		bullet_init(object);
	}

	if ((keystates[SDL_SCANCODE_E] || keystates[SDL_SCANCODE_KP_4] || keystates[SDL_SCANCODE_T]) && (time->time - time->teleport_time)/1000 >= 0.15)
	{
		time->teleport_time = SDL_GetTicks();
		object->player.origin.x = (rand() % (1024 - 1 + 1)) + 1;
		object->player.origin.y = (rand() % (768 - 1 + 1)) + 1;
	}
}

void Destroy_bullet(t_object* object, t_bullet* bullet)
{
	bullet->is_alive = 0;
	bullet->speed = 0;
	bullet->origin.x -= FLT_MAX;	
}

void bullet_movement(t_bullet* bullet)
{
	bullet->origin.x += bullet->speed * -bullet->J.x;
	bullet->origin.y += bullet->speed * -bullet->J.y;

	if (bullet->origin.x >= 1023)
	{
		bullet->origin.x = 0.1;
	}
	if (bullet->origin.x <= 0)
	{
		bullet->origin.x = 1024;
	}

	if (bullet->origin.y >= 767)
	{
		bullet->origin.y = 0.1;
	}

	if (bullet->origin.y <= 0)
	{
		bullet->origin.y = 768;
	}
	
}

void print_bullet(t_bullet* bullet, t_screen* screen)
{
	SDL_RenderDrawLines(screen->renderer, LocalToWorld_list(bullet->polygon.l_list,
															bullet->polygon.w_list,
															bullet->origin, 
															bullet->I, 
															bullet->J, 
															bullet->polygon.size), 
															bullet->polygon.size);
}

void Player_fire_process(t_object* object, t_screen* screen, t_time* time)
{
	t_bullet* bullet = first;

	while (bullet != NULL)
	{
		if (bullet->is_alive == 1)
		{
			bullet_movement(bullet);

			if (is_player_bullet_collide(object,bullet))
			{
				destroy_player(object);
			}
			print_bullet(bullet, screen);

			if ( (time->time - bullet->fired)/1000 >= 0.5)
			{
				Destroy_bullet(object, bullet);
			}

			for (unsigned int index_m = 0 ; index_m < screen->nb_mines ; index_m++)
			{
				if (is_mine_bullet_collide(object,bullet,index_m))
				{
					if (object->mine[index_m].type_m == 2 || object->mine[index_m].type_m == 4)
					{
						create_fireball(object,index_m);
					}
					
					object->mine[index_m].is_alive = 0;
					object->mine[index_m].origin.x -= FLT_MAX;
					object->nb_mines--;
					Destroy_bullet(object,bullet);

					object->score.score_int =  object->score.score_int + object->mine[index_m].score_value;

					if (object->mine[index_m].size > 1)
					{
						for (unsigned i = 0; i < 2; i++)
						{
							create_mine(object,object->mine[index_m].size-1,object->mine[index_m].type_m,(screen->nb_mines - object->Spawn_list.size));
						}
					}
					if (object->nb_mines == 0)
					{
						screen->win = true;
					}
					
				}
			}
		}
		bullet = bullet->next;
	}

	Fireball* fireball = third;
	bullet = first;

	while (fireball != NULL)
	{
		if (fireball->is_alive == 1)
		{
			bullet_movement(fireball);

			if (is_player_fireball_collide(object,fireball))
			{
				destroy_player(object);
			}
			SDL_SetRenderDrawColor(screen->renderer, 255,165,0, 255);
			print_bullet(fireball, screen);

			if ( (time->time - fireball->fired)/1000 >= 1)
			{
				Destroy_bullet(object, fireball);
			}
			while(bullet != NULL)
			{
				if (bullet->is_alive == 1)
				{
					if (is_bullet_fireball_collide(object,bullet,fireball))
					{
						Destroy_bullet(object, fireball);
						Destroy_bullet(object, bullet);
					}
				}
				bullet = bullet->next;
			}
		}
		fireball = fireball->next;
	}
}

void create_fireball(t_object* object, unsigned int index_m)
{
	Fireball* fireball = calloc(1, sizeof(Fireball));

	if ( third == NULL )
	{
		third				= fireball;
		fourth 				= fireball;
	    fireball->prev 		= NULL;
	}

	else
	{
		fireball->prev 			= fourth;
		fourth 					= fireball;
		fireball->prev->next 	= fireball;
	}
	
	fireball->polygon.size 		= 9;
	fireball->polygon.w_list 	= calloc(fireball->polygon.size, sizeof(SDL_Point));
	fireball->polygon.l_list 	= calloc(fireball->polygon.size, sizeof(SDL_Point));
	fireball->origin.x 			= object->mine[index_m].origin.x;
	fireball->origin.y			= object->mine[index_m].origin.y;
	fireball->I.x 				= -1 * object->player.I.x;
	fireball->J.x				= -1 * object->player.J.x;
	fireball->I.y 				= -1 * object->player.I.y;
	fireball->J.y				= -1 * object->player.J.y;
	fireball->radius 			= 6;
	fireball->fired 			= SDL_GetTicks();
	fireball->speed 			= 2;
	fireball->is_alive			= 1;
	fireball->next 				= NULL;
	unsigned int k 				= 0;
	float coords_list[] 		= {-fireball->radius ,0,
									-fireball->radius *cos(45),fireball->radius *sin(45),
									0,fireball->radius ,
									fireball->radius *cos(45),fireball->radius *sin(45),
									fireball->radius ,0,
									fireball->radius *cos(45),-fireball->radius *sin(45),
									0,-fireball->radius ,
									-fireball->radius *cos(45),-fireball->radius *sin(45),
									-fireball->radius ,0};
	for (unsigned int i = 0; i < 9; i++)
	{
		fireball->polygon.l_list[i].x = coords_list[k];
		fireball->polygon.l_list[i].y = coords_list[k+1];
		k += 2;
	}
}

void bullet_init(t_object* object)
{
	t_bullet* bullet = calloc(1, sizeof(t_bullet));

	if ( first == NULL )
	{
		first 				= bullet;
		second 				= bullet;
	    bullet->prev 		= NULL;
	}

	else
	{
		bullet->prev 		= second;
		second 				= bullet;
		bullet->prev->next 	= bullet;
	}
	
	bullet->polygon.size 	= 9;
	bullet->polygon.w_list 	= calloc(bullet->polygon.size, sizeof(SDL_Point));
	bullet->polygon.l_list 	= calloc(bullet->polygon.size, sizeof(SDL_Point));
	bullet->origin.x 		= object->player.polygon[0].w_list[2].x;
	bullet->origin.y		= object->player.polygon[0].w_list[2].y;
	bullet->I 				= object->player.I;
	bullet->J 				= object->player.J;
	bullet->fired 			= SDL_GetTicks();
	bullet->radius 			= 4;
	bullet->speed 			= object->player.speed/100 + 15;
	bullet->is_alive		= 1;
	bullet->next 			= NULL;
	unsigned int k 			= 0;
	float coords_list[] 	= {-bullet->radius,0,
								-bullet->radius*cos(45),bullet->radius*sin(45),
								0,bullet->radius,
								bullet->radius*cos(45),bullet->radius*sin(45),
								bullet->radius,0,
								bullet->radius*cos(45),-bullet->radius*sin(45),
								0,-bullet->radius,
								-bullet->radius*cos(45),-bullet->radius*sin(45),
								-bullet->radius,0};
	for (unsigned int i = 0; i < 9; i++)
	{
		bullet->polygon.l_list[i].x = coords_list[k];
		bullet->polygon.l_list[i].y = coords_list[k+1];
		k += 2;
	}
}


Point mine_movement(t_mine mine, t_object* object )
{
	if ( mine.type_m == 3 || mine.type_m == 4 )
	{
		if ( object->player.origin.x+10 > mine.origin.x )
		{
			mine.origin.x += mine.speed;
		}

		else if ( object->player.origin.x-10 < mine.origin.x)
		{
			mine.origin.x -= mine.speed;
		}
		
		if (object->player.origin.y+10 > mine.origin.y)
		{
			mine.origin.y += mine.speed;
		}
		
		else if (object->player.origin.y-10 < mine.origin.y)
		{
			mine.origin.y -= mine.speed;
		}
	}

	else if ( mine.type_m == 1 || mine.type_m == 2 )
	{
		mine.origin.x += mine.speed * mine.J.x;
		mine.origin.y += mine.speed * mine.J.y;
	}
	
	if (mine.origin.x >= 1023)
	{
		mine.origin.x = 1;
	}
	if (mine.origin.x <= 0)
	{
		mine.origin.x = 1022;
	}

	if (mine.origin.y >= 767)
	{
		mine.origin.y = 1;
	}

	if (mine.origin.y <= 0)
	{
		mine.origin.y = 766;
	}
	
	return (mine.origin);
}

void Game_destroy(t_object* object, t_time* time, t_screen* screen)
{
	t_bullet* bullet  = first;
	while (bullet)
	{
		Destroy_bullet(object, bullet);
		bullet = bullet->next;
	}
	screen->score = object->score.score_int;
	free(object);
	TTF_Quit();
}
