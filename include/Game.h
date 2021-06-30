#ifndef GAME_H
#define GAME_H

// include all the needed librairies
#include "main_process.h"

typedef struct s_polygon
{
	int 		size;
	SDL_Point* 	w_list;
	SDL_Point* 	l_list;
}t_polygon;

typedef struct s_bullet
{
	Point 				origin;
	Vector2 			I;
	Vector2 			J;
	float				fired;
	float	 			speed;
	t_polygon 			polygon;
	float 				radius;
	struct s_bullet* 	next;
	struct s_bullet* 	prev;
	unsigned int 		is_alive;
	


} t_bullet,Fireball;

typedef struct s_score
{
	SDL_Surface* 	surface;
	SDL_Rect 		rect;
	SDL_Texture* 	texture;
	char* 			score_text;
	int 			score_int;
} t_score;

typedef struct s_player
{
	Point	origin;
	Vector2		I;
	Vector2		J;
	Vector2 	Velocity;
	Vector2 	Acceleration;
	t_polygon* 	polygon; 
	float 		speed;
	bool 		is_alive;
	t_bullet* 		bullet;
	unsigned int 	life;
}t_player;

typedef struct s_mine
{
	t_polygon* 		polygon;
	float 			speed;
	unsigned int 	size;
	unsigned int 	type_m;
	Point 		origin;
	Vector2			I;
	Vector2			J;
	unsigned int 	nb_polygon;
	bool 			is_alive;
	int 			score_value;
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
	SDL_Point* 		list;
	unsigned int 	size;	
	Point*			origin_l; 
	unsigned int	list_size;
	SDL_Point*		l_list; 	
	SDL_Point*		w_list; 	
	Vector2			I;		
	Vector2			J;		
	float			radius;	
}Spawn_list;

typedef struct Control_Panel
{
	int* 			Control_list;
	unsigned int 	number_of_S3_T1;
	unsigned int 	number_of_S2_T1;
	unsigned int 	number_of_S1_T1;
	unsigned int 	number_of_S3_T2;
	unsigned int 	number_of_S2_T2;
	unsigned int 	number_of_S1_T2;
	unsigned int 	number_of_S3_T3;
	unsigned int 	number_of_S2_T3;
	unsigned int 	number_of_S1_T3;
	unsigned int 	number_of_S3_T4;
	unsigned int 	number_of_S2_T4;
	unsigned int 	number_of_S1_T4;
	unsigned int 	number_of_Fireball;
	unsigned int 	number_of_Minelayer;
}Control_Panel;

typedef struct s_object
{
	t_mine* 		mine;
	t_pic			pic;
	t_player		player;
	Spawn_list 		Spawn_list;
	Control_Panel 	Control_Panel;
	int 			nb_mines;
	t_score			score;
} t_object;

t_object* Game_init(t_screen* screen);

void Game_print_text(t_screen* screen, t_object* object,t_time* time);

void Game_destroy(t_object* object, t_time* time, t_screen* screen);

void Player_movement(t_object* object, SDL_Event event, t_time* time);

void Player_fire(t_object* object, t_time* time);

void init_screen(t_screen* screen);

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

void create_mine_list(t_object* object);

bool is_player_mine_collide(t_object* object, unsigned int index_m);

bool is_range_collide(t_polygon polygon , t_polygon polygon2, SDL_Point a, SDL_Point b);

void range_computing(Vector2 n, SDL_Point* list, unsigned int len, float* min, float* max);

bool is_polygon_collide(t_object* object, unsigned int index_m, unsigned int index_p , unsigned int i2);

void bullet_movement(t_bullet* bullet);

void print_bullet(t_bullet* bullet, t_screen* screen);

void Player_fire_process(t_object* object, t_screen* screen, t_time* time);

void bullet_init(t_object* object);

bool is_polygon_bullet_mine_collide(t_object* object, unsigned int index_m, unsigned int index_p, t_bullet* bullet);

bool is_player_bullet_collide(t_object* object, t_bullet* bullet);

bool is_polygon_bullet_player_collide(t_object* object, unsigned int i2, t_bullet* bullet);

bool is_polygon_collide(t_object* object, unsigned int index_m, unsigned int index_p , unsigned int i2);

bool is_range_collide(t_polygon polygon , t_polygon polygon2, SDL_Point a, SDL_Point b);

void range_computing(Vector2 n, SDL_Point* list, unsigned int len, float* min, float* max);

void Player_fire_teleport(t_object* object, t_time* time);

void Destroy_bullet(t_object* object, t_bullet* bullet);

void bullet_movement(t_bullet* bullet);

void print_bullet(t_bullet* bullet, t_screen* screen);

void Player_fire_process(t_object* object, t_screen* screen, t_time* time);

void bullet_init(t_object* object);

Point mine_movement(t_mine mine, t_object* object);

bool Is_game_paused(bool is_game_paused, t_time* time, t_screen* screen, t_object* object);

void text_pause(t_screen* screen);

void create_fireball(t_object* object, unsigned int index_m);

bool is_bullet_fireball_collide(t_object* object, t_bullet* bullet, Fireball* fireball);

bool is_player_fireball_collide(t_object* object, Fireball* bullet);

bool is_polygon_fireball_player_collide(t_object* object, unsigned int i2, Fireball* bullet);

void generate_minefield(t_screen* screen, t_object* object);

#endif
