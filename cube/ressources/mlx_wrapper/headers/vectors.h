/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: giuliovalente <giuliovalente@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:15 by giuliovalen       #+#    #+#             */
/*   Updated: 2025/05/02 09:51:56 by giuliovalen      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTORS_H
# define VECTORS_H

# include "../mlx_utils.h"

# define EPSILON 0.00001f

typedef struct s_Vector2
{
	int	x;
	int	y;
}	t_vec2;

typedef struct s_Vector2f
{
	float	x;
	float	y;
}	t_vec2f;

typedef struct s_Vector3
{
	int	x;
	int	y;
	int	z;
}	t_vec3;

typedef struct s_Vector3f
{
	float	x;
	float	y;
	float	z;
}	t_vec3f;

typedef struct s_Vector4
{
	int	r;
	int	g;
	int	b;
	int	a;
	int	x;
	int	y;
	int	z;
	int	w;
}	t_vec4;

typedef struct s_Vector4f
{
	float	r;
	float	g;
	float	b;
	float	a;
}	t_vec4f;

//		VECTOR_2.c
t_vec2	v2(int x, int y);
t_vec2	v3_to_v2(t_vec3 vec);
int		cmp_vec2(t_vec2 a, t_vec2 b);
int		print_vec2(t_vec2 a, const char *label);
int		v2_touch(t_vec2 a, t_vec2 a_size, t_vec2 b, t_vec2 b_size);

//		VECTOR_2f.c
t_vec2f	v3_to_v2f(t_vec3 vec);
int		cmp_vec2f(t_vec2f a, t_vec2f b, float margin);
t_vec2f	get_v2f(float x, float y);
int		print_vec2f(t_vec2f a, const char *label);

//		VECTOR_2f_b.c
t_vec2f	v2f_center(t_vec2f a, t_vec2f b);
int		v2f_bounds(t_vec2f a, t_vec2f bnd_pos, t_vec2f bnd_size);
t_vec2f	v2f(int scale);
int		v2f_touch(t_vec2f a, t_vec2f a_size, t_vec2f b, t_vec2f b_size);

//		VECTOR_3f.c
t_vec3f	get_v3f(float x, float y, float z);
int		cmp_vec3f(t_vec3f a, t_vec3f b, float margin);
int		print_vec3f(t_vec3f a, const char *label);

//		VECTOR_3.c
t_vec3	v3(int x, int y, int z);
t_vec3	v2_to_v3(t_vec2 vec);
int		cmp_vec3(t_vec3 a, t_vec3 b);
int		print_vec3(t_vec3 a, const char *label);

//		VECTOR_3f.c
t_vec3f	get_v3f(float x, float y, float z);
int		cmp_vec3f(t_vec3f a, t_vec3f b, float margin);
int		print_vec3f(t_vec3f a, const char *label);

//		VECTOR_4.c
t_vec4	v4(int r, int g, int b, int a);
t_vec3	v4_to_v3(t_vec4 a);
int		cmp_vec4(t_vec4 a, t_vec4 b);
int		print_vec4(t_vec4 a, const char *label);

//		VECTOR_4f.c
t_vec4f	get_v4f(float r, float g, float b, float a);
int		print_vec4f(t_vec4f a, const char *label);
t_vec3f	v4f_to_v3f(t_vec4f a);

//		VECTOR_b
t_vec2	v2_center(t_vec2 a, t_vec2 b);
t_vec3f	v3f_center(t_vec3f a, t_vec3f b);
t_vec3	v3_center(t_vec3 a, t_vec3 b);
int		v2_bounds(t_vec2 a, t_vec2 bnd_pos, t_vec2 bnd_size);
int		v3f_bounds(t_vec3f a, t_vec3f a_size, t_vec3f pos, t_vec3f b_size);
int		v3_bounds(t_vec3 a, t_vec3 pos, t_vec3 size);
t_vec3f	normalize_vec3f(t_vec3f v);

t_vec2	_v2(int scale);
t_vec3	_v3(int scale);
t_vec3f	v3f(float scale);
t_vec4	_v4(int scale);
t_vec4f	v4f(float scale);
int		v4_to_color(int r, int g, int b, int a);
t_vec4	color_to_v4(int color);

t_vec3f	scale_vec3f(t_vec3f v, float s);
t_vec3	scale_vec3(t_vec3 v, float s);
t_vec2f	scale_vec2f(t_vec2f v, float s);
t_vec2	scale_vec2(t_vec2 v, float s);

t_vec3f	add_vec3f(t_vec3f a, t_vec3f b);
t_vec3	add_vec3(t_vec3 a, t_vec3 b);
t_vec2f	add_vec2f(t_vec2f a, t_vec2f b);
t_vec2	add_vec2(t_vec2 a, t_vec2 b);

t_vec3f	sub_vec3f(t_vec3f a, t_vec3f b);
t_vec3	sub_vec3(t_vec3 a, t_vec3 b);
t_vec2f	sub_vec2f(t_vec2f a, t_vec2f b);
t_vec2	sub_vec2(t_vec2 a, t_vec2 b);

t_vec2	div_v2(t_vec2 a, int div);
t_vec2f	div_v2f(t_vec2f a, float div);
t_vec3	div_v3(t_vec3 a, int div);
t_vec3f	div_v3f(t_vec3f a, float div);
int		color_compare(int color, t_vec4 cmp);

#endif