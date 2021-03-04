#include <assert.h>
#include "Game.h"

void* first 	= NULL;
void* second 	= NULL;

void Game(t_screen* screen, t_time* time)
{	
	t_object* object = Game_init(screen);
	SDL_Event event;
	bool isGameRunning = true;
	while (isGameRunning)
	{
		//SDL_Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		time->time = SDL_GetTicks(); // current time in milliseconds 
		if (time->time > time->prevTime)
		{
			time->deltaTime = (time->time - time->prevTime) / 1000.f; // convert deltatime in seconds
			time->prevTime = time->time;
		}
		Player_movement(object, event, time);

		if (SDL_PollEvent(&event) == 1)
		{
			Player_fire(object,time);
			// allow the player to close the game with the escape keyboard touch
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				isGameRunning = false;
				Game_destroy(object);
				screen->screen_number = 0;
			}

		}
		if (isGameRunning)
			Game_print_text(screen, object);
	}


}

void init_screen(t_screen* screen)
{
	//clear the screen
	SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
	SDL_RenderClear(screen->renderer);
	SDL_SetWindowTitle(screen->screen, "((+_+))((+_+))MineStorm((+_+))((+_+))");
}

t_pic Map_init(t_screen* screen, t_object* object)
{
	t_pic pic 		= object->pic;
	pic.surface 	= SDL_LoadBMP("./media/Picture/New_Map.bmp");
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
	object->player.speed 					= 0.0;
	object->player.origin.x					= 200;
	object->player.origin.y					= 200;
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
	t_mine* mine 			= object->mine;
	mine[index].size 		= size;
	mine[index].type_m		= type_m;
	mine[index].speed 		= 10.0 * mine->size;
	mine[index].origin.x	= object->Spawn_list.list[object->Spawn_list.size - 1].x;
	mine[index].origin.y	= object->Spawn_list.list[object->Spawn_list.size - 1].y;
	mine[index].nb_polygon = 4;
	mine[index].is_alive = true;

	if ( type_m == 1)
	{
		mine[index].nb_polygon = 3;
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
}

void control_panel(Control_Panel* panel)
{
	panel->number_of_S3_T1 = 1;
	panel->number_of_S2_T1 = 1;
	panel->number_of_S1_T1 = 1;
	panel->number_of_S3_T2 = 1;
	panel->number_of_S2_T2 = 1;
	panel->number_of_S1_T2 = 1;
	panel->number_of_S3_T3 = 1;
	panel->number_of_S2_T3 = 1;
	panel->number_of_S1_T3 = 1;
	panel->number_of_S3_T4 = 1;
	panel->number_of_S2_T4 = 1;
	panel->number_of_S1_T4 = 1;
	panel->number_of_Fireball = 0;
	panel->number_of_Minelayer = 0;

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

	while ( index < Control_Panel.number_of_S2_T1 + index_nb )
	{
		create_mine(object, 2, 1, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S1_T1 + index_nb )
	{
		create_mine(object, 1, 1, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S3_T2 + index_nb )
	{
		create_mine(object, 3, 2, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S2_T2 + index_nb )
	{
		create_mine(object, 2, 2, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S1_T2 + index_nb )
	{
		create_mine(object, 1, 2, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S3_T3 + index_nb )
	{
		create_mine(object, 3, 3, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S2_T3 + index_nb )
	{
		create_mine(object, 2, 3, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S1_T3 + index_nb )
	{
		create_mine(object, 1, 3, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S3_T4 + index_nb )
	{
		create_mine(object, 3, 4, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S2_T4 + index_nb )
	{
		create_mine(object, 2, 4, index);
		index++;
	}

	index_nb = index;
	
	while ( index < Control_Panel.number_of_S1_T4 + index_nb )
	{
		create_mine(object, 1, 4, index);
		index++;
	}
}

t_object* Game_init(t_screen* screen)
{
	init_screen(screen);
	t_object* object = calloc(1, sizeof(t_object));
	object->mine = calloc(screen->nb_mines, sizeof(t_mine));
	object->Spawn_list.size = screen->nb_mines;
	object->Spawn_list.list = calloc(object->Spawn_list.size, sizeof(SDL_Point));
	object->player.polygon = calloc(2, sizeof(t_polygon));

	for ( unsigned int i = 0 ; i < 2 ; i++)
	{
		object->player.polygon[i].size = 4;
		object->player.polygon[i].w_list = calloc(object->player.polygon[i].size, sizeof(SDL_Point));
		object->player.polygon[i].l_list = calloc(object->player.polygon[i].size, sizeof(SDL_Point));
	}

	t_pic pic = Map_init(screen, object);
	
	Generate_random_spawn_pts(object, screen->nb_mines);
	player_init(object);
	control_panel(&object->Control_Panel);
	create_mine_list(object);
	object->pic = pic;

	return object;
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

void print_mines(t_object* object, t_screen* screen)
{
	t_mine* mine = object->mine;
	for ( unsigned int index = 0 ; index < screen->nb_mines ; index++ )
	{
		for (unsigned int i = 0; i < mine[index].nb_polygon ; i++)
		{
			mine[index].polygon[i].w_list = LocalToWorld_list(mine[index].polygon[i].l_list,
								mine[index].polygon[i].w_list,
								mine[index].origin, 
								mine[index].I, 
								mine[index].J, 
								mine[index].polygon[i].size);
		}
		if (object->mine[index].is_alive)
		{
			bool collide = is_player_mine_collide(object, index);
		
			if (!collide)
			{
				for (unsigned int i = 0; i < mine[index].nb_polygon ; i++)
				{
					SDL_RenderDrawLines(screen->renderer, mine[index].polygon[i].w_list, mine[index].polygon[i].size);
				}
			}
			else
			{
				mine[index].origin.x -= 10000000000;
				object->mine[index].is_alive = false;	
			}
		}
	}
}

void Generate_random_spawn_pts(t_object* object, unsigned int nb_mines)
{
	for ( unsigned int index = 0 ; index < nb_mines ; index++)
	{
		object->Spawn_list.list[index].x = (rand() % (1024 - 5 + 1)) + 5;
		object->Spawn_list.list[index].y = (rand() % (768 - 5 + 1)) + 5;
	}
}

void Game_print_text(t_screen* screen, t_object* object)
{
	SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
	SDL_RenderClear(screen->renderer);
	SDL_RenderCopy(screen->renderer, object->pic.texture, &(object->pic.src),&(object->pic.rect));
	SDL_SetRenderDrawColor(screen->renderer, 33, 237, 237, 255);
	print_player(object, screen);
	Player_fire_process(object,screen);
	SDL_SetRenderDrawColor(screen->renderer, 255, 0, 0, 0);
	print_mines(object,screen);
	SDL_SetRenderDrawColor(screen->renderer, 0, 0, 0, 255);
	SDL_RenderPresent(screen->renderer);
}

void Player_movement(t_object* object, SDL_Event event, t_time* time)
{
	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	if (keystates[SDL_SCANCODE_R])
	{
		object->player.speed += 50;
	}

	if (keystates[SDL_SCANCODE_D])
	{
		object->player.I = rotate_vector(object->player.I, 354);
		object->player.J = rotate_vector(object->player.I, 90);	
	}

	else if (keystates[SDL_SCANCODE_G])
	{
		object->player.I = rotate_vector(object->player.I, 6);
		object->player.J = rotate_vector(object->player.I, 90);	
	}

	if (object->player.speed > 0)
	{
		object->player.speed -= 10;
	}

	if (object->player.speed > 1000.0 )
	{
		object->player.speed = 1000.0;
	}
	
	object->player.Acceleration.x = object->player.speed * object->player.J.x * time->deltaTime;
	object->player.Acceleration.y = object->player.speed * object->player.J.y * time->deltaTime;
	//object->player.Acceleration = Unit_vector_computing(object->player.Acceleration);
	object->player.Velocity.x += object->player.Acceleration.x * time->deltaTime;
	object->player.Velocity.y += object->player.Acceleration.y * time->deltaTime;
	//object->player.Velocity = Unit_vector_computing(object->player.Velocity);
	if (keystates[SDL_SCANCODE_R] && vector_length(object->player.Velocity) <= 10)
	{
		object->player.origin.x -= object->player.Velocity.x; 
		object->player.origin.y -= object->player.Velocity.y;
	}

	else
	{
		object->player.origin.x -= object->player.Velocity.x; 
		object->player.origin.y -= object->player.Velocity.y;
		object->player.Velocity.x -= (object->player.Velocity.x/10);
		object->player.Velocity.y -= (object->player.Velocity.y/10);
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

		bool playerToMine = is_range_collide( object->player.polygon[i2],
								object->mine[index_m].polygon[index_p],
								object->player.polygon[i2].w_list[i],
								object->player.polygon[i2].w_list[i+1]);

		if ( !playerToMine )
		{
			return false;
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
	//printf("min_m = %f; min_p = %f; max_m = %f; max_p = %f;  \n", min_m[0], min_p[0], max_m[0], max_p[0]);
	if ( max_m[0] < min_p[0] )
	{
		free(min_m);
		free(max_m);
		free(min_p);
		free(max_p);
		//printf("X \n");
		return false;
	}

	if ( min_m[0] > max_p[0] ) 
	{
		free(min_m);
		free(max_m);
		free(min_p);
		free(max_p);
		//printf("X \n");
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
		//printf("SDL_Point = {%i, %i} \n", list[index].x, list[index].y);
	}

	float* r_list = malloc(len * sizeof(float));
	float a;
	for (unsigned int  i = 0; i < len ; i++)
	{
		a = dot_product(v_list[i], n);
		//printf("point = {%f, %f} \n", v_list[i].x, v_list[i].y);
		//printf("dot = %f\n", a);
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
	//printf("YYYYYYYYYYY \n");
	min[0] = min_range;
	max[0] = max_range;
	free(v_list);
	free(r_list);
}

void Player_fire(t_object* object, t_time* time)
{
	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	if (keystates[SDL_SCANCODE_F] && (time->time - time->fire_time)/1000 >= 0.15)
	{
		time->fire_time = SDL_GetTicks();
		bullet_init(object);
	}
}

void Destroy_bullet(t_object* object)
{
	t_bullet* bullet = first;
	first = bullet->next;
	bullet->next->prev = NULL;
	bullet->next = NULL;
	bullet->speed = 0;
	bullet->origin.x -= FLT_MAX;
	free(bullet->polygon.w_list);
	free(bullet->polygon.w_list);
	free(bullet);

}

void bullet_movement(t_bullet* bullet)
{
	bullet->origin.x += bullet->speed * -bullet->J.x;
	bullet->origin.y += bullet->speed * -bullet->J.y;
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

void Player_fire_process(t_object* object, t_screen* screen)
{
	t_bullet* bullet = first;

	while ( bullet != NULL )
	{
		bullet_movement(bullet);
		//is_bullet_collide(bullet);
		print_bullet(bullet, screen);
		bullet = bullet->next;
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
	bullet->radius 			= 2.8;
	bullet->speed 			= object->player.speed/100 + 15;
	float rad_div 			= 1.4;
	bullet->next 			= NULL;
	unsigned int k 			= 0;
	float coords_list[] 	= {-bullet->radius,0,
								-bullet->radius/rad_div,bullet->radius/rad_div,
								0,bullet->radius,
								bullet->radius/rad_div,bullet->radius/rad_div,
								bullet->radius,0,
								bullet->radius/rad_div,-bullet->radius/rad_div,
								0,-bullet->radius,
								-bullet->radius/rad_div,-bullet->radius/rad_div,
								-bullet->radius,0};
	for (unsigned int i = 0; i < 9; i++)
	{
		bullet->polygon.l_list[i].x = coords_list[k];
		bullet->polygon.l_list[i].y = coords_list[k+1];
		k += 2;
	}
}

void Game_destroy(t_object* object)
{
	free(object);
	TTF_Quit();
}
