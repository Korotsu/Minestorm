#ifndef MY_MATHS_H
#define MY_MATHS_H
#define M_PI	3.14159265358979323846
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
//#include "Game.h"

typedef struct Vector2
{
	float x;
	float y;
} Vector2,Point;

typedef struct s_line
{
	Point 	a;
	Point 	b;

} t_line;

Vector2 create_null_vector();

Vector2 Unit_vector_computing(Vector2 vector);

bool Is_equal(Vector2 vector1 , Vector2 vector2 );

float vector_length(Vector2 vector);

float square_length(Vector2 vector);

float distance_between_pts(Point a, Point b);

Vector2 vector_addition(Vector2 vector1, Vector2 vector2);

Vector2 vector_substraction(Vector2 vector1, Vector2 vector2);

Vector2 vector_scale(Vector2 vector1, float a);

Vector2 vector_negate(Vector2 vector1);

Vector2 rotate_vector(Vector2 vector1, float angle );

Vector2 normal_rotate_90_vector(Vector2 vector1);

float dot_product(Vector2 vector1, Vector2 vector2);

int find_angle(Vector2 vector1, Vector2 vector2);

#endif
