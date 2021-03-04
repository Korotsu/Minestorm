#ifndef GAME_H
#define GAME_H

// include all the needed librairies
#include "main_process.h"

typedef struct s_bullet
{
	Point 		origin;
	Vector2 	I;
	Vector2 	J;
	bool 		is_alive;
	Vector2 	speed;
	t_polygon 	polygon;
	float 		radius;
	

} t_bullet;

/*typedef struct s_score
{
} t_score;*/

typedef struct s_polygon
{
	int 		size;
	SDL_Point* 	w_list;
	SDL_Point* 	l_list;
}t_polygon;

typedef struct s_player
{
	Point	 	origin;
	Vector2		I;
	Vector2		J;
	Vector2 	Velocity;
	Vector2 	Acceleration;
	t_polygon* 	polygon; 
	float 		speed;
	bool 		is_alive;
}t_player;

typedef struct s_mine
{
	t_polygon* 		polygon;
	float 			speed;
	unsigned int 	size;
	unsigned int 	type_m;
	Point 			origin;
	Vector2			I;
	Vector2			J;
	unsigned int 	nb_polygon;
	bool 			is_alive;
} t_mine;

typedef struct s_pic
{
	SDL_Surface* 	surface;
	SDL_Texture* 	texture;
	SDL_Rect 		rect;
	SDL_Rect 		src;

} t_pic;

typedef struct Spawn_list
{
	SDL_Point* list;
	unsigned int size;
}Spawn_list;

typedef struct Control_Panel
{
	unsigned int number_of_S3_T1;
	unsigned int number_of_S2_T1;
	unsigned int number_of_S1_T1;
	unsigned int number_of_S3_T2;
	unsigned int number_of_S2_T2;
	unsigned int number_of_S1_T2;
	unsigned int number_of_S3_T3;
	unsigned int number_of_S2_T3;
	unsigned int number_of_S1_T3;
	unsigned int number_of_S3_T4;
	unsigned int number_of_S2_T4;
	unsigned int number_of_S1_T4;
	unsigned int number_of_Fireball;
	unsigned int number_of_Minelayer;
}Control_Panel;

typedef struct s_object
{
	t_mine* 	mine;
	t_pic		pic;
	t_player	player;
	Spawn_list 	Spawn_list;
	Control_Panel Control_Panel;
	//t_bullet* bullet;
} t_object;

t_object* Game_init(t_screen* screen);

void Game_print_text(t_screen* screen, t_object* object);

void Game_destroy(t_object* object);

void Player_movement(t_object* object, SDL_Event event, t_time* time);

void Monsters_movement(t_object* object, t_time* time);

void collumns_alive(t_object* object);

void player_fire(t_object* object, SDL_Event event, t_time* time);

void player_fire_process(t_object* object, SDL_Event event, t_time* time);

void fire_ennemies_collide(t_object* object, unsigned int index);

void Font_print(t_object* object, t_screen* screen);

void init_screen(t_screen* screen);

void invaders_init(t_object* object, t_screen* screen);

void player_init(t_object* object);

t_pic Map_init(t_screen* screen, t_object* object);

void print_player(t_object* object, t_screen* screen);

SDL_Point LocalToWorld_pts(SDL_Point pts, Point origin , Vector2 I, Vector2 J);

SDL_Point WorldToLocal_pts(SDL_Point pts, Point origin, Vector2 I, Vector2 J);

Vector2 LocalToWorld_vector(Vector2 vector, t_object* object);

Vector2 WorldToLocal_vector(Vector2 vector, t_object* object);

SDL_Point* LocalToWorld_list(SDL_Point* list, SDL_Point* w_list, Point origin, Vector2 I, Vector2 J, unsigned int size);

SDL_Point* WorldToLocal_list(SDL_Point* list, Point origin, Vector2 I, Vector2 J, unsigned int size);

void Generate_random_spawn_pts(t_object* object, unsigned int nb_mines);

void create_mine(t_object* object, unsigned int size, unsigned int type_m, unsigned int index);

bool is_player_mine_collide(t_object* object, unsigned int index_m);

bool is_range_collide(t_polygon polygon , t_polygon polygon2, SDL_Point a, SDL_Point b);

void range_computing(Vector2 n, SDL_Point* list, unsigned int len, float* min, float* max);

bool is_polygon_collide(t_object* object, unsigned int index_m, unsigned int index_p , unsigned int i2);

#endif
